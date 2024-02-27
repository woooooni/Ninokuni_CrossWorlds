#include "stdafx.h"
#include "Dummy.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

CDummy::CDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_DUMMY)
{
}

CDummy::CDummy(const CDummy& rhs)
	: CGameObject(rhs)
{

}

HRESULT CDummy::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CDummy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDummy::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_AnimShader"), (CComponent**)&m_pAnimShaderCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_NonAnim_Shader"), (CComponent**)&m_pNonAnimShaderCom)))
			return E_FAIL;
	}

	return S_OK;
}


void CDummy::Tick(_float fTimeDelta)
{
	Input(fTimeDelta);
}

void CDummy::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pModelCom)
		return;
	
	__super::LateTick(fTimeDelta);

	if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // DEBUG

}

HRESULT CDummy::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	/* 공용 데이터 */
	CShader* pShader = m_pModelCom->Get_ModelType() == CModel::TYPE::TYPE_NONANIM ? m_pNonAnimShaderCom : m_pAnimShaderCom;

	if (nullptr == pShader)
		return S_OK;

	if (FAILED(pShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	/* 코스튬 작업시 */
	if (m_bCostumeMode)
	{
		if (FAILED(m_pModelCom->SetUp_VTF(pShader)))
			return E_FAIL;

		for (size_t i = 0; i < PART_TYPE::PART_END; i++)
		{
			if (nullptr == m_pPart[i])
				continue;

			const _uint		iNumMeshes = m_pPart[i]->Get_NumMeshes();

			for (_uint j = 0; j < iNumMeshes; ++j)
			{
				if (FAILED(m_pPart[i]->SetUp_OnShader(pShader, m_pPart[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pPart[i]->Render(pShader, j)))
					return E_FAIL;
			}
		}

		return S_OK;
	}

	/* 코스튬 작업 아닐시 */
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	if (0 < m_pModelCom->Get_Animations().size() && m_pModelCom->Get_ModelType() == CModel::TYPE_ANIM)
	{
		if (FAILED(m_pModelCom->SetUp_VTF(pShader)))
			return E_FAIL;
	}

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(pShader, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDummy::Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr != m_pModelCom)
		Safe_Release(m_pModelCom);	

	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	if (FAILED(GI->Import_Model_Data(LEVEL_DUMMY, wstring(L"Prototype_Componenet_Model_") + szFileName, eType, strFilePath, strFileName, &m_pModelCom)))
		return E_FAIL;


	m_pModelCom->Set_Owner(this);

	return S_OK;
}


HRESULT CDummy::Export_Model_Bin(const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr == m_pModelCom)
		return E_FAIL;
	
	if (FAILED(GI->Export_Model_Data(m_pModelCom, strFilePath, strFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDummy::Import_Model()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CDummy::Add_Collider(CCollider::COLLIDER_TYPE eColliderType, CCollider::DETECTION_TYPE eDetectionType)
{
	switch (eColliderType)
	{
	case Engine::CCollider::SPHERE:
	{
		CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
		ZeroMemory(&SphereDesc, sizeof SphereDesc);

		BoundingSphere tSphere;
		ZeroMemory(&tSphere, sizeof(BoundingSphere));
		tSphere.Radius = 1.f;
		SphereDesc.tSphere = tSphere;

		SphereDesc.pNode = nullptr;
		SphereDesc.pOwnerTransform = m_pTransformCom;
		SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
		SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

		if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, eDetectionType, &SphereDesc)))
			return E_FAIL;
	}
		break;
	case Engine::CCollider::OBB:
	{
		CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
		ZeroMemory(&OBBDesc, sizeof OBBDesc);

		BoundingOrientedBox OBBBox;
		ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

		XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
		OBBBox.Extents = { 50.f, 50.f, 50.f };

		OBBDesc.tBox = OBBBox;
		OBBDesc.pNode = nullptr;
		OBBDesc.pOwnerTransform = m_pTransformCom;
		OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
		OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

		if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, eDetectionType, &OBBDesc)))
			return E_FAIL;
	}
		break;
	case Engine::CCollider::TYPE_END:
		break;
	default:
		break;
	}
	return S_OK;
}

void CDummy::Input(_float fTimeDelta)
{

}


CDummy* CDummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CDummy* pInstance = new CDummy(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CDummy");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CDummy::Clone(void* pArg)
{
	CDummy* pInstance = new CDummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDummy::Free()
{
	__super::Free();

	Safe_Release(m_pNonAnimShaderCom);
	Safe_Release(m_pAnimShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	// Safe_Release(m_pModelCom);
}



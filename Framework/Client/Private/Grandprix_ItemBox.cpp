#include "stdafx.h"
#include "Grandprix_ItemBox.h"
#include "GameInstance.h"
#include "Grandprix_Manager.h"

CGrandprix_ItemBox::CGrandprix_ItemBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, LAYER_TYPE::LAYER_PROP)
{
}

CGrandprix_ItemBox::CGrandprix_ItemBox(const CGrandprix_ItemBox& rhs)
	: CGameObject(rhs)
{

}

void CGrandprix_ItemBox::Set_MinMaxPosition(Vec4 vOriginPos)
{
	_float fRandom = GI->RandomFloat(0.5f, 1.3f);
	
	m_vMinHeight = vOriginPos.y - fRandom;
	m_vMaxHeight = vOriginPos.y + fRandom;

	m_fSpeed = GI->RandomFloat(0.5f, 1.5f);
	m_bReverse = GI->RandomInt(0, 1);

	// 아이템 타입을 랜덤으로 세팅한다.
	switch (GI->RandomInt(0, ITEMBOX_END - 1))
	{
	case ITEMBOX_TYPE::ITEMBOX_SPEEDUP:
		m_eItemType = ITEMBOX_SPEEDUP;
		break;

	case ITEMBOX_TYPE::ITEMBOX_SIZEUP:
		m_eItemType = ITEMBOX_SIZEUP;
		break;

	case ITEMBOX_TYPE::ITEMBOX_BOMB:
		m_eItemType = ITEMBOX_BOMB;
		break;

	case ITEMBOX_TYPE::ITEMBOX_SLOW:
		m_eItemType = ITEMBOX_SLOW;
		break;
	}
}

HRESULT CGrandprix_ItemBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGrandprix_ItemBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(Vec3(2.f, 2.f, 2.f));

	m_bActive = false;

	return S_OK;
}

void CGrandprix_ItemBox::Tick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::Tick(fTimeDelta);

	Update_Position(fTimeDelta);
	Update_Rotation(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_ITEM, this);
}

void CGrandprix_ItemBox::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CGrandprix_ItemBox::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint iPassIndex = 0;
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CGrandprix_ItemBox::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;

	_float4 vCamPosition = GI->Get_CamPosition();

	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
			return E_FAIL;
	}

	return S_OK;
}

void CGrandprix_ItemBox::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER)
	{
		CGrandprix_Manager::GetInstance()->Add_ItemBox(m_eItemType);
		Set_Dead(true);
	}
}

void CGrandprix_ItemBox::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_ItemBox::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_ItemBox::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);

}

void CGrandprix_ItemBox::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CGrandprix_ItemBox::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CGrandprix_ItemBox::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Grandprix_ItemBox"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CGrandprix_ItemBox::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 2.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

void CGrandprix_ItemBox::Update_Position(_float fTimeDelta)
{
	Vec4 vCurPosition = m_pTransformCom->Get_Position();
	if (true == m_bReverse)
	{
		if (vCurPosition.y >= m_vMaxHeight)
			m_bReverse = false;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPosition, vCurPosition.y += m_fSpeed * fTimeDelta));
	}
	else
	{
		if (vCurPosition.y <= m_vMinHeight)
			m_bReverse = true;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPosition, vCurPosition.y -= m_fSpeed * fTimeDelta));
	}
}

void CGrandprix_ItemBox::Update_Rotation(_float fTimeDelta)
{
//	fRoll += XMConvertToRadians(45.f * fTimeDelta);
//	fPitch += XMConvertToRadians(45.f * fTimeDelta);
//	fYaw += XMConvertToRadians(45.f * fTimeDelta);

	m_vRotationAngle.x += XMConvertToRadians(45.f * fTimeDelta);
	m_vRotationAngle.y += XMConvertToRadians(45.f * fTimeDelta);
	m_vRotationAngle.z += XMConvertToRadians(45.f * fTimeDelta);

	// 각 축에 대한 쿼터니언 생성
	Vec4 vRoll = XMQuaternionRotationRollPitchYaw(m_vRotationAngle.x, 0.0f, 0.0f);
	Vec4 vPitch = XMQuaternionRotationRollPitchYaw(0.0f, m_vRotationAngle.y, 0.0f);
	Vec4 vYaw = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, m_vRotationAngle.z);

	// 각 축의 쿼터니언을 곱하여 최종 회전 쿼터니언 얻음
	Vec4 vQuaternion = XMQuaternionMultiply(vYaw, XMQuaternionMultiply(vPitch, vRoll));

	// 쿼터니언을 행렬로 변환
	_matrix rotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	Vec3 vScale = m_pTransformCom->Get_Scale();

	// 행렬을 상태에 적용
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, rotationMatrix.r[0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, rotationMatrix.r[1] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, rotationMatrix.r[2] * vScale.z);
}

#pragma endregion

CGrandprix_ItemBox* CGrandprix_ItemBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrandprix_ItemBox* pInstance = new CGrandprix_ItemBox(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CGrandprix_ItemBox");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGrandprix_ItemBox::Clone(void* pArg)
{
	CGrandprix_ItemBox* pInstance = new CGrandprix_ItemBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CGrandprix_ItemBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrandprix_ItemBox::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
}


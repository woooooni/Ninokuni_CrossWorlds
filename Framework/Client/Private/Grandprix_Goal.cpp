#include "stdafx.h"
#include "Grandprix_Goal.h"
#include "GameInstance.h"
#include "Grandprix_Manager.h"
#include "UIMinigame_Manager.h"
#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

CGrandprix_Goal::CGrandprix_Goal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Grandprix_Goal", LAYER_TYPE::LAYER_PROP)
{
}

CGrandprix_Goal::CGrandprix_Goal(const CGrandprix_Goal& rhs)
	: CGameObject(rhs)
{

}

void CGrandprix_Goal::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
		m_bStartDissolve = true;
		m_fDissolveWeight = 10.f;
//		m_vDissolveColor = _float4(0.715f, 0.995f, 1.f, 1.f);
		m_vDissolveColor = _float4(0.212f, 0.620f, 1.f, 1.f);
	}

	m_bActive = bActive;
}

HRESULT CGrandprix_Goal::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Goal::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(Vec3(30.f, 30.f, 30.f));
	m_pTransformCom->Set_Position(Vec4(-0.5f, 111.5f, 288.f, 1.f));

	m_vBloomPower = _float3(1.f, 1.f, 1.f);

	// Dissolve Texture
	m_pDissoveTexture = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise")));
	if (m_pDissoveTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

void CGrandprix_Goal::Tick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::Tick(fTimeDelta);

	// Start Dissolve
	if (true == m_bStartDissolve)
	{
		m_fDissolveWeight -= m_fDissolveSpeed * fTimeDelta;

		if (m_fDissolveWeight < 0.f)
		{
			m_fDissolveWeight = 0.f;
			m_bStartDissolve = false;
		}
	}

	m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_ITEM, this);
}

void CGrandprix_Goal::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

//	m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CGrandprix_Goal::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 1.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	// Dissolve --------------------------------------------------------------------
	if (FAILED(m_pDissoveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	// -----------------------------------------------------------------------------

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

//		if (m_bStartDissolve || m_bReserveDead)
			iPassIndex = 8;
//		else
//			iPassIndex = 0;
//		else if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
//			iPassIndex = 0;
//		else
//			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGrandprix_Goal::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

void CGrandprix_Goal::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		// 여기서 다시 퀘스트로 돌아간다.
		
		
		// UI 복구, 남은 관련 클래스 Set Dead
		CUIMinigame_Manager::GetInstance()->End_Grandprix();
		Set_Dead(true);
	}
}

void CGrandprix_Goal::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_Goal::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrandprix_Goal::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

//	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
//		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Grandprix_SiriusRelic"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Goal::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere; 
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 6.5f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 30.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CGrandprix_Goal* CGrandprix_Goal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGrandprix_Goal* pInstance = new CGrandprix_Goal(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CGrandprix_Goal");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGrandprix_Goal::Clone(void* pArg)
{
	CGrandprix_Goal* pInstance = new CGrandprix_Goal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CGrandprix_Goal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrandprix_Goal::Free()
{
	__super::Free();

	Safe_Release(m_pDissoveTexture);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
}


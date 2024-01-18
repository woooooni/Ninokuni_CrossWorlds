#include "stdafx.h"
#include "GameInstance.h"
#include "Cannon_Tower.h"
#include "Monster.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "UIDamage_Manager.h"
#include "Utils.h"
#include <future>

#include "Game_Manager.h"
#include "UI_Manager.h"
#include <MonsterProjectile.h>

USING(Client)
CCannon_Tower::CCannon_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_DEFENCE_TOWER)
{

}

CCannon_Tower::CCannon_Tower(const CCannon_Tower& rhs)
	: CGameObject(rhs)
{	

}

HRESULT CCannon_Tower::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCannon_Tower::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;



	return S_OK;
}

void CCannon_Tower::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::DEFENCE_TOWER, this);
}




void CCannon_Tower::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (nullptr == m_pRendererCom)
		return;

	m_pBarrelModelCom->LateTick(fTimeDelta);
	m_pBaseModelCom->LateTick(fTimeDelta);

		
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

HRESULT CCannon_Tower::Render()
{
	if (nullptr == m_pBarrelModelCom || nullptr == m_pBaseModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pBarrelModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	const _uint		iNumMeshes = m_pBarrelModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pBarrelModelCom->SetUp_OnShader(m_pShaderCom, m_pBarrelModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pBarrelModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCannon_Tower::Render_ShadowDepth()
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	if (FAILED(m_pBarrelModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	const _uint		iNumMeshes = m_pBarrelModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pBarrelModelCom->SetUp_OnShader(m_pShaderCom, m_pBarrelModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pBarrelModelCom->Render(m_pShaderCom, i, 10)))
			return E_FAIL;
	}


	return S_OK;
}

void CCannon_Tower::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CCannon_Tower::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CCannon_Tower::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CCannon_Tower::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CCannon_Tower::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CCannon_Tower::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

void CCannon_Tower::Look_For_Target(_bool bEnemy)
{
	
}

HRESULT CCannon_Tower::Ready_Components()
{
	return S_OK;
}

HRESULT CCannon_Tower::Ready_States()
{
	return S_OK;
}

HRESULT CCannon_Tower::Ready_Colliders()
{
	return S_OK;
}

void CCannon_Tower::On_Damaged(const COLLISION_INFO& tInfo)
{

}






CCannon_Tower* CCannon_Tower::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag)
{
	CCannon_Tower* pInstance = new CCannon_Tower(pDevice, pContext, strObjTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed. : CCannon_Tower");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CCannon_Tower::Clone(void* pArg)
{
	CCannon_Tower* pInstance = new CCannon_Tower(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Failed. : CCannon_Tower");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCannon_Tower::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Defence_Tower.h"
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
CDefence_Tower::CDefence_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_DEFENCE_TOWER)
{

}

CDefence_Tower::CDefence_Tower(const CDefence_Tower& rhs)
	: CGameObject(rhs)
{	

}

HRESULT CDefence_Tower::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDefence_Tower::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CDefence_Tower::Tick(_float fTimeDelta)
{
	if (false == m_bInitMatrix)
	{
		m_BaseMatrix = m_pTransformCom->Get_WorldMatrix();
		m_bInitMatrix = true;
	}

	__super::Tick(fTimeDelta);
	Tick_Target(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::DEFENCE_TOWER, this);
}




void CDefence_Tower::LateTick(_float fTimeDelta)
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

HRESULT CDefence_Tower::Render()
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

	_uint		iNumMeshes = m_pBarrelModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pBarrelModelCom->SetUp_OnShader(m_pShaderCom, m_pBarrelModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pBarrelModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	if (FAILED(m_pBaseModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;





	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_BaseMatrix.Transpose(), sizeof(_float4x4))))
		return E_FAIL;


	iNumMeshes = m_pBaseModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pBaseModelCom->SetUp_OnShader(m_pShaderCom, m_pBaseModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pBaseModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDefence_Tower::Render_ShadowDepth()
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

void CDefence_Tower::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CDefence_Tower::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);

	if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		{
			if(m_pStateCom->Get_CurrState() != DEFENCE_TOWER_STATE::TOWER_STATE_DEAD)
				Decide_Target(tInfo);
		}
	}
}

void CDefence_Tower::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CDefence_Tower::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CDefence_Tower::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CDefence_Tower::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

void CDefence_Tower::Look_For_Target(_bool bEnemy)
{
	if (nullptr == m_pTarget)
		return;

	if (true == bEnemy)
	{
		if (m_pTarget->Get_ObjectType() != OBJ_TYPE::OBJ_MONSTER && m_pTarget->Get_ObjectType() != OBJ_TYPE::OBJ_BOSS)
			return;
	}

	CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTargetTransform)
		return;

	m_pTransformCom->LookAt_ForLandObject(XMVectorSetW(pTargetTransform->Get_Position(), 1.f));
}


void CDefence_Tower::On_Damaged(const COLLISION_INFO& tInfo)
{

}

void CDefence_Tower::Tick_Target(_float fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;


	if (m_pTarget->Is_ReserveDead() || m_pTarget->Is_Dead())
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
		return;
	}

	CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr != pTargetTransform)
	{
		Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
		if (vDir.Length() > 6.f)
		{
			Safe_Release(m_pTarget);
			m_pTarget = nullptr;
		}
	}
}

void CDefence_Tower::Decide_Target(COLLISION_INFO tInfo)
{
	if (nullptr == m_pTarget)
	{
		m_pTarget = tInfo.pOther;
		Safe_AddRef(m_pTarget);
	}
	else
	{
		if (m_pTarget->Is_ReserveDead() || m_pTarget->Is_Dead())
		{
			Safe_Release(m_pTarget);
			m_pTarget = nullptr;
			return;
		}

		CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
		CTransform* pNewTargetTransform = tInfo.pOther->Get_Component<CTransform>(L"Com_Transform");

		if (nullptr != pTargetTransform)
		{
			Vec3 vTargetDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
			Vec3 vNewTargetDir = pNewTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
			if (vNewTargetDir.Length() < vTargetDir.Length())
			{
				if (nullptr != m_pTarget)
				{
					Safe_Release(m_pTarget);
					m_pTarget = nullptr;
				}
				m_pTarget = tInfo.pOther;
				Safe_AddRef(m_pTarget);
			}
		}
	}
}


void CDefence_Tower::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	
	Safe_Release(m_pBarrelModelCom);
	Safe_Release(m_pBaseModelCom);

	Safe_Release(m_pStateCom);
	Safe_Release(m_pTarget);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pTransformCom);

	
}

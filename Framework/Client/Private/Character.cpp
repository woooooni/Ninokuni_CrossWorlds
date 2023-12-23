#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "HierarchyNode.h"
#include "Key_Manager.h"
#include "Part.h"
#include "Sword.h"
#include "Trail.h"
#include "Monster.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "Utils.h"
#include "Weapon.h"
#include <future>

USING(Client)
CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
	, m_eCharacterType(eCharacterType)
{

}

CCharacter::CCharacter(const CCharacter& rhs)
	: CGameObject(rhs)
	, m_eCharacterType(rhs.m_eCharacterType)
{	

}

HRESULT CCharacter::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	for (_uint i = 0; i < SOCKET_END; ++i)
		m_pTrails[i] = nullptr;

	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		m_tStat = *((CHARACTER_STAT*)pArg);


	for (_uint i = 0; i < SOCKET_END; ++i)
		m_pTrails[i] = nullptr;

	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		m_pCharacterPartModels[i] = nullptr;


	return S_OK;
}

void CCharacter::Tick(_float fTimeDelta)
{
	GI->Add_CollisionGroup(COLLISION_GROUP::CHARACTER, this);

	if(nullptr != m_pWeapon)
		m_pWeapon->Tick(fTimeDelta);
	//if (m_bInfinite)
	//{
	//	m_fAccInfinite += fTimeDelta;
	//	if (m_fAccInfinite >= m_fInfiniteTime)
	//	{
	//		m_bInfinite = false;
	//		m_fAccInfinite = 0.f;

	//		Set_ActiveColliders(CCollider::DETECTION_TYPE::HEAD, true);
	//		Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, true);
	//	}
	//}
	//

	//for(_uint i = 0; i < SOCKET_END; ++i)
	//{
	//	if (nullptr == m_pTrails[i])
	//		continue;

	//	_matrix		WorldMatrix = m_Sockets[i]->Get_CombinedTransformation() * m_pModelCom->Get_PivotMatrix();

	//	WorldMatrix.r[CTransform::STATE_RIGHT] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_RIGHT]);
	//	WorldMatrix.r[CTransform::STATE_UP] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_UP]);
	//	WorldMatrix.r[CTransform::STATE_LOOK] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_LOOK]);

	//	m_pTrails[i]->Set_TransformMatrix(WorldMatrix * m_pTransformCom->Get_WorldMatrix());
	//	m_pTrails[i]->Tick(fTimeDelta);
	//}
}

void CCharacter::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;

	m_pModelCom->LateTick(fTimeDelta);

	if (nullptr != m_pWeapon)
		m_pWeapon->LateTick(fTimeDelta);
		
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);



#ifdef _DEBUG
	//m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	//{
	//	for (auto& pCollider : m_Colliders[i])
	//		m_pRendererCom->Add_Debug(pCollider);
	//}
	//m_pRendererCom->Add_Debug(m_pNavigationCom);
	//m_pRendererCom->Add_Debug(m_pRigidBodyCom);
#endif
}

HRESULT CCharacter::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
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
	if (m_bInfinite)
	{
		vRimColor = { 0.f, 0.5f, 1.f, 1.f };
	}
		

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	
	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	

	return S_OK;
}

HRESULT CCharacter::Render_ShadowDepth()
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;


	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j, 10)))
				return E_FAIL;
		}
	}


	return S_OK;
}

void CCharacter::Collision_Enter(const COLLISION_INFO& tInfo)
{
	
}

void CCharacter::Collision_Continue(const COLLISION_INFO& tInfo)
{
	
}

void CCharacter::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

void CCharacter::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
	m_pRigidBodyCom->Set_Ground(true);
	m_pRigidBodyCom->Set_Use_Gravity(false);
}

void CCharacter::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CCharacter::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	m_pRigidBodyCom->Set_Ground(false);
 	m_pRigidBodyCom->Set_Use_Gravity(true);
}





void CCharacter::Generate_Trail(SOCKET_TYPE eSocketType)
{
	if (eSocketType >= SOCKET_TYPE::SOCKET_END)
		return;

	if (nullptr != m_pTrails[eSocketType])
		m_pTrails[eSocketType]->Start_Trail(m_pTransformCom->Get_WorldMatrix());
}

void CCharacter::Stop_Trail(SOCKET_TYPE eSocketType)
{
	if (eSocketType >= SOCKET_TYPE::SOCKET_END)
		return;

	if (nullptr != m_pTrails[eSocketType])
		m_pTrails[eSocketType]->Stop_Trail();

}


void CCharacter::Set_Infinite(_float fInfiniteTime, _bool bInfinite)
{
	m_bInfinite = bInfinite;
	m_fInfiniteTime = fInfiniteTime;
	m_fAccInfinite = 0.f;
}


void CCharacter::On_Damaged(const COLLISION_INFO& tInfo)
{

}


void CCharacter::Free()
{
	__super::Free();

	/*for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		Safe_Release(m_pCharacterPartModels[i]);*/



	for (_uint i = 0; i < SOCKET_END; ++i)
	{
		if (nullptr == m_pTrails[i])
			continue;

		Safe_Release(m_pTrails[i]);
	}

	Safe_Release(m_pWeapon);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pNavigationCom);
}

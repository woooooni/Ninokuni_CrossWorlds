#include "stdafx.h"
#include "GameInstance.h"
#include "Biplane_BlackHole.h"

#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Vehicle_Flying.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

CBiplane_BlackHole::CBiplane_BlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVehicleFlying_Projectile(pDevice, pContext, L"Biplane_BlackHole", OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE)
{

}

CBiplane_BlackHole::CBiplane_BlackHole(const CBiplane_BlackHole& rhs)
	: CVehicleFlying_Projectile(rhs)
{
}


HRESULT CBiplane_BlackHole::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBiplane_BlackHole::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fMoveSpeed = 10.f;
	m_pTransformCom->Set_Scale(Vec3(0.15f, 0.15f, 0.15f));

	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);

	m_fDeletionTime = 5.f;


	//if (FAILED(CEffect_Manager::GetInstance()->Generate_Effect(L"Effect_GrandPrix_BlackHole_Sphere", m_pTransformCom->Get_WorldMatrix(),
	//	Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this)))
	//	return E_FAIL;
	//
	//if (FAILED(CEffect_Manager::GetInstance()->Generate_Effect(L"Effect_GrandPrix_AccretionDisk", m_pTransformCom->Get_WorldMatrix(),
	//	Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this)))
	//	return E_FAIL;
	//
	//if (FAILED(CEffect_Manager::GetInstance()->Generate_Effect(L"Effect_GrandPrix_AccretionDisk_Distortion", m_pTransformCom->Get_WorldMatrix(),
	//	Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this)))
	//	return E_FAIL;
	//
	//if (FAILED(CParticle_Manager::GetInstance()->Generate_Particle(L"Particle_BlackHole", m_pTransformCom->Get_WorldMatrix(), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this)))
	//	return E_FAIL;

	return S_OK;
}

void CBiplane_BlackHole::Tick(_float fTimeDelta)
{

	if (m_pBlackHole == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_GrandPrix_BlackHole"), m_pTransformCom->Get_WorldMatrix(), this, &m_pBlackHole);
	}

	if (true == m_bArrive)
	{
		__super::Tick(fTimeDelta);
		m_fAccColliderOnOffDamage += fTimeDelta;
		if (m_fAccColliderOnOffDamage >= m_fColliderOnOffDamageTime)
		{
			m_fAccColliderOnOffDamage = 0.f;
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, !Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active());
		}
		
	}
	else
	{
		m_fAccArrive += fTimeDelta;
		if (m_fAccArrive >= m_fArriveTime)
		{
			m_bArrive = true;
			m_fAccArrive = 0.f;
			m_fAccDeletionTime = 0.f;
		
			for (auto& pBodyCollider : Get_Collider(CCollider::DETECTION_TYPE::BODY))			
				pBodyCollider->Set_Radius(50.f);
		
			for (auto& pAttackCollider : Get_Collider(CCollider::DETECTION_TYPE::ATTACK))
				pAttackCollider->Set_Radius(50.f);
		
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
		}
		else
		{
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
		}
	}
	
	

	
}

void CBiplane_BlackHole::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_PROJECTILE, this);

#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CBiplane_BlackHole::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}





HRESULT CBiplane_BlackHole::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Biplane_BlackHole"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;*/


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = XMMatrixRotationY(180.f);
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;
	
	
	SphereDesc.tSphere.Radius = 30.f;
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CBiplane_BlackHole::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
	if (OBJ_TYPE::OBJ_GRANDPRIX_ENEMY == tInfo.pOther->Get_ObjectType())
	{
		CTransform* pTargetTransform = tInfo.pOther->Get_Component_Transform();
		if (nullptr != pTargetTransform)
		{
			Vec3 vGravityDir = XMVector3Normalize(m_pTransformCom->Get_Position() - pTargetTransform->Get_Position());
			pTargetTransform->Move(vGravityDir, m_fGravityForce, GI->Get_TimeDelta(TIMER_TYPE::GAME_PLAY));
		}
	}
}


CBiplane_BlackHole* CBiplane_BlackHole::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBiplane_BlackHole* pInstance = new CBiplane_BlackHole(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CBiplane_BlackHole");
		Safe_Release<CBiplane_BlackHole*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CBiplane_BlackHole::Clone(void* pArg)
{
	CBiplane_BlackHole* pInstance = new CBiplane_BlackHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CBiplane_BlackHole");
		Safe_Release<CBiplane_BlackHole*>(pInstance);
	}

	return pInstance;
}

void CBiplane_BlackHole::Free()
{
	__super::Free();
}

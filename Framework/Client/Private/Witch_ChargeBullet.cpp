#include "stdafx.h"
#include "Witch_ChargeBullet.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "DreamMazeWitch_Npc.h"

#include "Particle_Manager.h"
#include "Particle.h"

CWitch_ChargeBullet::CWitch_ChargeBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_ChargeBullet::CWitch_ChargeBullet(const CWitch_ChargeBullet& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_ChargeBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWitch_ChargeBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_Stat().eElementType);
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, true);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	m_fDelteTime = 6.f;
	m_fSpeed = 10.f;

	return S_OK;
}

void CWitch_ChargeBullet::Tick(_float fTimeDelta)
{
	// 이펙트 생성
	if (!m_bCreate)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall"),
			m_pTransformCom->Get_WorldMatrix(), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this);

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall_Flame"),
			m_pTransformCom->Get_WorldMatrix(), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this);

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall_Lighting"),
			m_pTransformCom->Get_WorldMatrix(), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), this);

		m_bCreate = true;
	}


	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fSpeed, fTimeDelta);

	m_fAccDelteTime += fTimeDelta;
	// 지우기
	if (m_fAccDelteTime > m_fDelteTime)
	{
		Set_Dead(true);
	}


	__super::Tick(fTimeDelta);
}

void CWitch_ChargeBullet::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWitch_ChargeBullet::Render()
{
	return S_OK;
}

HRESULT CWitch_ChargeBullet::Render_ShadowDepth()
{
	//if (FAILED(__super::Render_ShadowDepth()))
	//	return E_FAIL;

	return S_OK;
}


void CWitch_ChargeBullet::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall_Hit"),
			m_pTransformCom->Get_WorldMatrix(), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f));

		Set_Dead(this);
	}
}

void CWitch_ChargeBullet::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CWitch_ChargeBullet::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CWitch_ChargeBullet::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	if (m_pOwner != nullptr)
	{
		m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

		/* For.Com_Transform */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
			return E_FAIL;

		m_pWitch = m_pOwner;

		m_pTransformCom->Set_WorldMatrix(m_pWitch->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldMatrix());

		Vec4 vDarkBallPos = m_pTransformCom->Get_Position();
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vDarkBallPos);
	}
	else
	{
		MSG_BOX("Fail Create : Witch VulcanBullet , m_pOwner == nullptr");
		return E_FAIL;
	}


	/* For.Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_GlanixIceBall"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	//	return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 2.f, 0.f };
	ControllerDesc.fRaidus = 2.f;
	ControllerDesc.fHeight = 5.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.pOwner = this;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
	//	return E_FAIL;
	//
	//CRigidBody::RIGID_BODY_DESC RigidDesc;
	//RigidDesc.pTransform = m_pTransformCom;
	//
	///* For. Com_RigidBody*/
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
	//	return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CWitch_ChargeBullet::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.5f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = {};
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CWitch_ChargeBullet* CWitch_ChargeBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_ChargeBullet* pInstance = new CWitch_ChargeBullet(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_ChargeBullet");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_ChargeBullet::Clone(void* pArg)
{
	CWitch_ChargeBullet* pInstance = new CWitch_ChargeBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_ChargeBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_ChargeBullet::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "Witch_VulcanBullet.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "DreamMazeWitch_Npc.h"

#include "Effect_Manager.h"
#include "Particle_Manager.h"

CWitch_VulcanBullet::CWitch_VulcanBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_VulcanBullet::CWitch_VulcanBullet(const CWitch_VulcanBullet& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_VulcanBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWitch_VulcanBullet::Initialize(void* pArg)
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
	m_fReturnTime = 3.f;
	m_fDelteTime = 6.f;
	m_fSpeed = 10.f;

	// 이펙트 생성
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Marble"), m_pTransformCom->Get_WorldMatrix(), this);

	return S_OK;
}

void CWitch_VulcanBullet::Tick(_float fTimeDelta)
{
	m_fAccReturnTime += fTimeDelta;
	// 다시 반대로 돌아오기
	if (!m_bIsReturn && m_fAccReturnTime > m_fReturnTime)
	{
		m_bIsReturn = true;
	}

	if (m_bIsReturn)
	{
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), -m_fSpeed, fTimeDelta);
	
		m_fAccDelteTime += fTimeDelta;
		// 지우기
		if (m_fAccDelteTime > m_fDelteTime)
		{
			Set_Dead(true);
		}
	}
	else
	{
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fSpeed, fTimeDelta);
	}


	__super::Tick(fTimeDelta);
}

void CWitch_VulcanBullet::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWitch_VulcanBullet::Render()
{
	return S_OK;
}

HRESULT CWitch_VulcanBullet::Render_ShadowDepth()
{
	//if (FAILED(__super::Render_ShadowDepth()))
	//	return E_FAIL;

	return S_OK;
}


void CWitch_VulcanBullet::Collision_Enter(const COLLISION_INFO& tInfo)
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

void CWitch_VulcanBullet::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CWitch_VulcanBullet::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CWitch_VulcanBullet::Ready_Components()
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
HRESULT CWitch_VulcanBullet::Ready_Colliders()
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

CWitch_VulcanBullet* CWitch_VulcanBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_VulcanBullet* pInstance = new CWitch_VulcanBullet(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_VulcanBullet");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_VulcanBullet::Clone(void* pArg)
{
	CWitch_VulcanBullet* pInstance = new CWitch_VulcanBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_VulcanBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_VulcanBullet::Free()
{
	__super::Free();
}


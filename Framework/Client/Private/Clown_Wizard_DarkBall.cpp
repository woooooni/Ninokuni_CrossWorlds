#include "stdafx.h"
#include "Clown_Wizard_DarkBall.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Clown_Wizard.h"

CClown_Wizard_DarkBall::CClown_Wizard_DarkBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CClown_Wizard_DarkBall::CClown_Wizard_DarkBall(const CClown_Wizard_DarkBall& rhs)
	: CMonsterProjectile(rhs)
	, m_fTime(0.f)
	, m_fDelteTime(rhs.m_fDelteTime)
{

}

HRESULT CClown_Wizard_DarkBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CClown_Wizard_DarkBall::Initialize(void* pArg)
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
	m_fTime = 0.f;
	m_fDelteTime = 5.f;
	m_fSpeed = 10.f;

	return S_OK;
}

void CClown_Wizard_DarkBall::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	m_pTransformCom->Move(XMVector3Normalize(m_vInitLook), m_fSpeed, fTimeDelta);

	__super::Tick(fTimeDelta);

	if (m_fTime > m_fDelteTime)
	{
		m_fTime = m_fDelteTime - m_fTime;
		// Reserve_Dead(true);
		Set_Dead(true);
	}

}

void CClown_Wizard_DarkBall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CClown_Wizard_DarkBall::Render()
{
	return S_OK;
}

HRESULT CClown_Wizard_DarkBall::Render_ShadowDepth()
{
	//if (FAILED(__super::Render_ShadowDepth()))
	//	return E_FAIL;

	return S_OK;
}


void CClown_Wizard_DarkBall::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	//if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
	//	tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
	//	tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	//{
	//	Set_Dead(this);
	//}
}

void CClown_Wizard_DarkBall::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CClown_Wizard_DarkBall::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CClown_Wizard_DarkBall::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_pOwner != nullptr)
	{
		/* For.Com_Transform */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
			return E_FAIL;

		m_pClown_Wizard = dynamic_cast<CClown_Wizard*>(m_pOwner);

		m_pTransformCom->Set_WorldMatrix(m_pClown_Wizard->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldMatrix());
		
		Vec4 vDarkBallPos = m_pTransformCom->Get_Position();
		vDarkBallPos.y += 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vDarkBallPos);
		// m_pTransformCom->Set_Scale(Vec3(1.f, 1.f, 1.f));

		m_vInitLook = XMVector3Normalize(m_pTransformCom->Get_Look());
	}
	else
	{
		MSG_BOX("Fail Create : Clown_Wizard DarkBall, m_pOwner == nullptr");
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
HRESULT CClown_Wizard_DarkBall::Ready_Colliders()
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

CClown_Wizard_DarkBall* CClown_Wizard_DarkBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CClown_Wizard_DarkBall* pInstance = new CClown_Wizard_DarkBall(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CClown_Wizard_DarkBall");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CClown_Wizard_DarkBall::Clone(void* pArg)
{
	CClown_Wizard_DarkBall* pInstance = new CClown_Wizard_DarkBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CClown_Wizard_DarkBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_Wizard_DarkBall::Free()
{
	__super::Free();
}


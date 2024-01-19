#include "stdafx.h"
#include "Glanix_ShockWave.h"

#include "GameInstance.h"
#include "Glanix.h"

CGlanix_ShockWave::CGlanix_ShockWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CGlanix_ShockWave::CGlanix_ShockWave(const CGlanix_ShockWave& rhs)
	: CMonsterProjectile(rhs)
	, m_fTime(0.f)
	, m_fDelteTime(rhs.m_fDelteTime)

{
}

HRESULT CGlanix_ShockWave::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_ShockWave::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_Stat().eElementType);
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::AIR_BORNE, 0.f, 0.f, 0.f, true);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	m_fTime = 0.f;
	m_fDelteTime = 10.f;
	fSpeed = 0.f;

	return S_OK;
}

void CGlanix_ShockWave::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	fSpeed += fTimeDelta;

	m_pTransformCom->Move(XMVector3Normalize(m_vInitLook), fSpeed * 2.f, fTimeDelta);

	__super::Tick(fTimeDelta);

	if (m_fTime > m_fDelteTime)
	{
		m_fTime = m_fDelteTime - m_fTime;
		Reserve_Dead(true);
		Set_Dead(true);
	}
}

void CGlanix_ShockWave::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CGlanix_ShockWave::Render()
{
	return S_OK;
}

HRESULT CGlanix_ShockWave::Render_ShadowDepth()
{
	return S_OK;
}

void CGlanix_ShockWave::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CGlanix_ShockWave::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CGlanix_ShockWave::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CGlanix_ShockWave::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CGlanix_ShockWave::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CGlanix_ShockWave::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CGlanix_ShockWave::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_pOwner != nullptr)
	{
		/* For.Com_Transform */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
			return E_FAIL;

		m_pGlanix = dynamic_cast<CGlanix*>(m_pOwner);

		m_pTransformCom->Set_WorldMatrix(m_pGlanix->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldMatrix());

		Vec4 vWavePos = m_pTransformCom->Get_Position();
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWavePos);
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

HRESULT CGlanix_ShockWave::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 100.f, 0.5f, 0.5f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = {};
	OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	/* ATK */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;


	return S_OK;
}

CGlanix_ShockWave* CGlanix_ShockWave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGlanix_ShockWave* pInstance = new CGlanix_ShockWave(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGlanix_ShockWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGlanix_ShockWave::Clone(void* pArg)
{
	CGlanix_ShockWave* pInstance = new CGlanix_ShockWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGlanix_ShockWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_ShockWave::Free()
{
	__super::Free();
}

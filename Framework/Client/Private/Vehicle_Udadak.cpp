#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Udadak.h"

#include "State_Vehicle_Idle.h"
#include "State_Vehicle_Jump.h"
#include "State_Vehicle_Run.h"
#include "State_Vehicle_Walk.h"
#include "State_Vehicle_Sprint.h"

#include "Character.h"

CVehicle_Udadak::CVehicle_Udadak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVehicle(pDevice, pContext, strObjectTag, OBJ_VEHICLE)
{
}

CVehicle_Udadak::CVehicle_Udadak(const CVehicle_Udadak& rhs)
	: CVehicle(rhs)
{
}

HRESULT CVehicle_Udadak::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVehicle_Udadak::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_bUseBone = true;
	m_fOffsetY = -1.5f;

	return S_OK;
}

void CVehicle_Udadak::Tick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::Tick(fTimeDelta);

		Update_RiderState();

		if (nullptr != m_pRigidBodyCom)
			m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

		if (nullptr != m_pControllerCom)
			m_pControllerCom->Tick_Controller(fTimeDelta);
	}
}

void CVehicle_Udadak::LateTick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::LateTick(fTimeDelta);

		Update_Rider(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
}

HRESULT CVehicle_Udadak::Render()
{
	if (true == m_bOnBoard)
	{
		__super::Render();
	}

	return S_OK;
}

HRESULT CVehicle_Udadak::Render_ShadowDepth()
{
	if (true == m_bOnBoard)
	{
		__super::Render_ShadowDepth();
	}

	return S_OK;
}

void CVehicle_Udadak::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle_Udadak::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle_Udadak::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle_Udadak::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
	if (nullptr != m_pRigidBodyCom)
	{
		if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
		{
			m_pRigidBodyCom->Set_Ground(true);
			m_pRigidBodyCom->Set_Use_Gravity(false);
		}
	}
}

void CVehicle_Udadak::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
	m_pRigidBodyCom->Set_Ground(true);
	m_pRigidBodyCom->Set_Use_Gravity(false);
}

void CVehicle_Udadak::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	m_pRigidBodyCom->Set_Ground(false);
	m_pRigidBodyCom->Set_Use_Gravity(true);
}

HRESULT CVehicle_Udadak::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Udadak"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Udadak::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Stand");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Stand02");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Idle01");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Idle02");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_Vehicle_Idle::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Walk");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_WALK, CState_Vehicle_Walk::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	//strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Run");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Sprint");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN, CState_Vehicle_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Sprint"); // 사용하지 않음.
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_SPRINT, CState_Vehicle_Sprint::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpStart");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpUp");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpDown");
	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpEnd");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_JUMP, CState_Vehicle_Jump::Create(m_pStateCom, strAnimationNames));

	m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);

	return S_OK;
}

void CVehicle_Udadak::Update_RiderState()
{
	if (nullptr != m_pRider)
	{
		CStateMachine* pStateCom = m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
		if (nullptr == pStateCom)
			return;

		if (CVehicle::VEHICLE_STATE::VEHICLE_RUN == m_pStateCom->Get_CurrState() &&
			CCharacter::STATE::VEHICLE_RUNSTART != pStateCom->Get_CurrState())
		{
			if (CCharacter::STATE::VEHICLE_RUN != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::VEHICLE_RUN);
		}

		if (CVehicle::VEHICLE_STATE::VEHICLE_WALK == m_pStateCom->Get_CurrState())
		{
			if (CCharacter::STATE::VEHICLE_STAND != pStateCom->Get_CurrState() &&
				CCharacter::STATE::VEHICLE_RUNSTART != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::VEHICLE_STAND);
		}

		if (CVehicle::VEHICLE_STATE::VEHICLE_IDLE == m_pStateCom->Get_CurrState())
		{
			if (CCharacter::STATE::VEHICLE_STAND != pStateCom->Get_CurrState() &&
				CCharacter::STATE::VEHICLE_RUNSTART != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::VEHICLE_STAND);
		}

		if (CVehicle::VEHICLE_STATE::VEHICLE_JUMP == m_pStateCom->Get_CurrState())
		{
			if (CCharacter::STATE::VEHICLE_RUN != pStateCom->Get_CurrState() &&
				CCharacter::STATE::VEHICLE_RUNSTART != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::VEHICLE_RUN);
		}
	}
}

HRESULT CVehicle_Udadak::Ready_Colliders()
{
//	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
//	ZeroMemory(&OBBDesc, sizeof OBBDesc);
//
//	BoundingOrientedBox OBBBox;
//	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));
//
//	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
//	OBBBox.Extents = { 35.f , 60.f, 40.f };
//
//	OBBDesc.tBox = OBBBox;
//	OBBDesc.pNode = nullptr;
//	OBBDesc.pOwnerTransform = m_pTransformCom;
//	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
//	OBBDesc.vOffsetPosition = Vec3(5.f, 60.f, 5.f);
//
//	/* Body */
//	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
//		return E_FAIL;
//
//	/* Atk */
//	OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);
//	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
//		return E_FAIL;
//
//	/* Boundary */
//	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
//	ZeroMemory(&SphereDesc, sizeof SphereDesc);
//	
//	BoundingSphere tSphere;
//	ZeroMemory(&tSphere, sizeof(BoundingSphere));
//	tSphere.Radius = 0.8f;
//	SphereDesc.tSphere = tSphere;
//	
//	SphereDesc.pNode = nullptr;
//	SphereDesc.pOwnerTransform = m_pTransformCom;
//	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
//	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);
//	
//	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
//		return E_FAIL;
//
	return S_OK;
}

CVehicle_Udadak* CVehicle_Udadak::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVehicle_Udadak* pInstance = new CVehicle_Udadak(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CVehicle_Udadak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVehicle_Udadak::Clone(void* pArg)
{
	CVehicle_Udadak* pInstance = new CVehicle_Udadak(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CVehicle_Udadak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVehicle_Udadak::Free()
{
	__super::Free();
}

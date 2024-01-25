#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "..\Public\Vehicle.h"

CVehicle::CVehicle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_VEHICLE)
{
}

CVehicle::CVehicle(const CVehicle& rhs)
	: CGameObject(rhs)
{

}

HRESULT CVehicle::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_VehicleDesc = *((VEHICLE_DESC*)(pArg));
	}



	return S_OK;
}

void CVehicle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_Rider(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::ANIMAL, this);
}

void CVehicle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	

	Compute_CamZ(m_pTransformCom->Get_Position());
}

HRESULT CVehicle::Render()
{


	return S_OK;
}

HRESULT CVehicle::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

void CVehicle::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle::Ride(CGameObject* pRider)
{
	if (nullptr == pRider)
	{
		MSG_BOX("Rider is Null :  CVehicle::Ride");
		return;
	}

	m_pRiderTransform = pRider->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == m_pRiderTransform)
	{
		MSG_BOX("Get Rider Transform Failed. :  CVehicle::Ride");
		return;
	}

	m_pRider = pRider;
}

void CVehicle::Update_Rider(_float fTimeDelta)
{
	if (nullptr == m_pRider || nullptr == m_pRiderTransform || m_pRider->Is_Dead() || m_pRider->Is_ReserveDead())
	{
		m_pRider = nullptr;
		m_pRiderTransform = nullptr;
		return;
	}
		
	Matrix SitBoneMatrix = m_pModelCom->Get_SocketLocalMatrix(0);

	if(nullptr != m_pRiderTransform)
		m_pRiderTransform->Set_WorldMatrix(SitBoneMatrix * m_pTransformCom->Get_WorldMatrix());
}

HRESULT CVehicle::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"),
		reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidDesc)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	return S_OK;
}

void CVehicle::Free()
{
	__super::Free();
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

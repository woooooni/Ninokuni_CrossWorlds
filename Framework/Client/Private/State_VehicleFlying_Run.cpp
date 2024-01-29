#include "stdafx.h"
#include "GameInstance.h"

#include "State_VehicleFlying_Run.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Camera_Follow.h"

CState_VehicleFlying_Run::CState_VehicleFlying_Run(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_VehicleFlying_Run::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_VehicleFlying_Run::Tick_State(_float fTimeDelta)
{
	_bool bMove = false;

	if (KEY_HOLD(KEY::W))
	{
		bMove = true;

		_matrix CamWorld = GI->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW);
		Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		Vec3 vCamLook = XMVector3Normalize(CamWorld.r[CTransform::STATE_LOOK]);
		vLook = XMVectorLerp(vLook, vCamLook, fTimeDelta);

		m_pTransformCom->Rotation_Look(vLook);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f * XMConvertToRadians(90.f) * fTimeDelta);
		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);

		bMove = true;
	}


	if (KEY_HOLD(KEY::D))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f) * fTimeDelta);
		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);

		bMove = true;
	}

	if (KEY_HOLD(KEY::Q))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(vRight, fRadian);
		m_pTransformCom->Move(vCamLook, m_pVehicle->Get_Speed(), fTimeDelta);
	}

//	if (KEY_TAP(KEY::SPACE))
//	{
//		bMove = true;
//		CUI_Manager::GetInstance()->Use_JumpBtn();
//		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_JUMP);
//		return;
//	}
//
//
//	if (KEY_NONE(KEY::SHIFT))
//	{
//		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_WALK);
//		return;
//	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
			return;
		}
	}
}

void CState_VehicleFlying_Run::Exit_State()
{

}

CState_VehicleFlying_Run* CState_VehicleFlying_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Run* pInstance = new CState_VehicleFlying_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Run::Free()
{
    __super::Free();
}

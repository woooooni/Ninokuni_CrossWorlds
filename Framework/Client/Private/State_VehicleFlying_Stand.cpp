#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_VehicleFlying_Stand.h"
#include "Vehicle.h"

CState_VehicleFlying_Stand::CState_VehicleFlying_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;
    
    return S_OK;
}

void CState_VehicleFlying_Stand::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(false);
}

void CState_VehicleFlying_Stand::Tick_State(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
	{
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
		return;
	}

//    if (KEY_TAP(KEY::SPACE))
//    {
//        CUI_Manager::GetInstance()->Use_JumpBtn();
//        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_JUMP);
//        return;
//    }

    if (true == GI->Mouse_Down(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(true);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(true);
    }

    if (true == GI->Mouse_Up(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(false);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(false);
    }
}

void CState_VehicleFlying_Stand::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    
    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);
}



CState_VehicleFlying_Stand* CState_VehicleFlying_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Stand* pInstance = new CState_VehicleFlying_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Stand::Free()
{
    __super::Free();
}

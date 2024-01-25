#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "State_Vehicle_Idle.h"
#include "UI_Manager.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_Vehicle_Idle::CState_Vehicle_Idle(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Vehicle_Idle::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    if(CGame_Manager::GetInstance()->Get_Kuu() != nullptr)
        CGame_Manager::GetInstance()->Get_Kuu()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CKuu::NPC_IDLE);

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(false);
}

void CState_Vehicle_Idle::Tick_State(_float fTimeDelta)
{
   
}

void CState_Vehicle_Idle::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);
}



CState_Vehicle_Idle* CState_Vehicle_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Idle* pInstance = new CState_Vehicle_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Idle::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Vehicle_Run.h"
#include "Riding_Manager.h"

CState_Character_Vehicle_Run::CState_Character_Vehicle_Run(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Vehicle_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Vehicle_Run::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_pRigidBodyCom->Set_Use_Gravity(false);
    m_pRigidBodyCom->Set_Ground(true);
}

void CState_Character_Vehicle_Run::Tick_State(_float fTimeDelta)
{
    /*
    _int iRandom = GI->RandomInt(0, 3);
    switch (iRandom)
    {
    case 0:
        GI->Play_Sound(TEXT("Veh_Udadak_Fear_Breathe_1_1.mp3"), CHANNELID::SOUND_VEHICLE,
            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
        break;

    case 1:
        GI->Play_Sound(TEXT("Veh_Udadak_Fear_Breathe_1_2.mp3"), CHANNELID::SOUND_VEHICLE,
            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
        break;

    case 2:
        GI->Play_Sound(TEXT("Veh_Udadak_Fear_Breathe_1_3.mp3"), CHANNELID::SOUND_VEHICLE,
            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
        break;

    case 3:
        GI->Play_Sound(TEXT("Veh_Udadak_Fear_Breathe_1_4.mp3"), CHANNELID::SOUND_VEHICLE,
            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
        break;
    }
    */
    m_pRigidBodyCom->Set_Use_Gravity(false);
    m_pRigidBodyCom->Set_Ground(true);

    __super::OnBoard_Input(fTimeDelta);
}

void CState_Character_Vehicle_Run::Exit_State()
{
    m_pRigidBodyCom->Set_Use_Gravity(true);
    m_pRigidBodyCom->Set_Ground(false);
}



CState_Character_Vehicle_Run* CState_Character_Vehicle_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Vehicle_Run* pInstance = new CState_Character_Vehicle_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Vehicle_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Vehicle_Run::Free()
{
    __super::Free();
}

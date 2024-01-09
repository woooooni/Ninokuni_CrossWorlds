#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Dummy_Swordsman.h"
#include "State_Lobby_SwordMan_Sit.h"

CState_Lobby_SwordMan_Sit::CState_Lobby_SwordMan_Sit(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Lobby_SwordMan_Sit::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;

    m_pSwordsman = dynamic_cast<CUI_Dummy_Swordsman*>(m_pCharacter);

    return S_OK;
}

void CState_Lobby_SwordMan_Sit::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Lobby_SwordMan_Sit::Tick_State(_float fTimeDelta)
{
    if (m_pSwordsman->Is_Clicked())
    {
        if (m_iCurrAnimIndex != m_AnimIndices[3])
        {
            m_iCurrAnimIndex = m_AnimIndices[3];

            GI->Stop_Sound(CHANNELID::SOUND_VOICE_CHARACTER);

            switch (GI->RandomInt(0, 1))
            {
            case 0:
                GI->Play_Sound(TEXT("Swordsman_System_Character_Choice_1.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
                break;

            case 1:
                GI->Play_Sound(TEXT("Swordsman_System_Character_Choice_2.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
                break;
            }
            
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
        }

        if (m_iCurrAnimIndex == m_AnimIndices[3] &&
            false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
            return;
        }
    }
    else
    {
        if (m_iCurrAnimIndex == m_AnimIndices[0]) // 23, 160, 161, 24
        {
//            m_fSitMotionTime = 2.f;
//            m_fSitTimeAcc += fTimeDelta;
//
//            if (m_fSitTimeAcc >= m_fSitMotionTime)
//            {
                if (false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
                {
                    m_fSitTimeAcc = 0.f;
                    m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 2)];
                    m_pModelCom->Set_Animation(m_iCurrAnimIndex, 0.5f);
                }
//            }
        }

        if (m_iCurrAnimIndex != m_AnimIndices[0] &&
            false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 2)];

            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
        }
    }
}

void CState_Lobby_SwordMan_Sit::Exit_State()
{
    m_fSitTimeAcc = 0;
    m_iCurrAnimIndex = 0;
}

CState_Lobby_SwordMan_Sit* CState_Lobby_SwordMan_Sit::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Lobby_SwordMan_Sit* pInstance = new CState_Lobby_SwordMan_Sit(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Lobby_SwordMan_Sit");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Lobby_SwordMan_Sit::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Dummy_Engineer.h"
#include "State_Lobby_Engineer_Sit.h"

CState_Lobby_Engineer_Sit::CState_Lobby_Engineer_Sit(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Lobby_Engineer_Sit::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;

    m_pEngineer = dynamic_cast<CUI_Dummy_Engineer*>(m_pCharacter);

    return S_OK;
}

void CState_Lobby_Engineer_Sit::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Lobby_Engineer_Sit::Tick_State(_float fTimeDelta)
{
    if (m_pEngineer->Is_Clicked())
    {
        if (m_iCurrAnimIndex != m_AnimIndices[4])
        {
            m_iCurrAnimIndex = m_AnimIndices[4];
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
        }

        if (m_iCurrAnimIndex == m_AnimIndices[4] &&
            false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
            return;
        }
    }
    else
    {
        if (m_iCurrAnimIndex == m_AnimIndices[0])
        {
             if (false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
             {
                 m_fSitTimeAcc = 0.f;
                 m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 3)];
                 m_pModelCom->Set_Animation(m_iCurrAnimIndex, 0.5f);
             }
        }

        if (m_iCurrAnimIndex != m_AnimIndices[0] &&
            false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 3)];

            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
        }
    }
}

void CState_Lobby_Engineer_Sit::Exit_State()
{
    m_fSitTimeAcc = 0;
    m_iCurrAnimIndex = 0;
}

CState_Lobby_Engineer_Sit* CState_Lobby_Engineer_Sit::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Lobby_Engineer_Sit* pInstance = new CState_Lobby_Engineer_Sit(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Lobby_Engineer_Sit");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Lobby_Engineer_Sit::Free()
{
    __super::Free();
}

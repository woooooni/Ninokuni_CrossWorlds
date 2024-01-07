#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Dummy_Destroyer.h"
#include "State_Lobby_Destroyer_Neutral_Idle.h"

CState_Lobby_Destroyer_Neutral_Idle::CState_Lobby_Destroyer_Neutral_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Lobby_Destroyer_Neutral_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;

    m_pDestroyer = dynamic_cast<CUI_Dummy_Destroyer*>(m_pCharacter);

    return S_OK;
}

void CState_Lobby_Destroyer_Neutral_Idle::Enter_State(void* pArg)
{
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Lobby_Destroyer_Neutral_Idle::Tick_State(_float fTimeDelta)
{
    if (!m_pDestroyer->Is_Clicked())
    {
        if (m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::LOBBY_SIT);
            return;
        }
    }
    else
    {
        m_fAccIdleMotion += fTimeDelta;

 //       if (m_iCurrAnimIndex != m_AnimIndices[0] &&
 //           false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        if (false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            if (m_fAccIdleMotion >= m_fIdleMotionTime)
            {
                m_fAccIdleMotion = 0.f;
                m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(0, 2)];

                m_pModelCom->Set_Animation(m_iCurrAnimIndex);
            }
        }

//       if (m_fAccIdleMotion >= m_fIdleMotionTime)
//       {
//           m_fAccIdleMotion = 0.f;
//           m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(0, 2)];
//
//           m_pModelCom->Set_Animation(m_iCurrAnimIndex);
//       }
    }
}

void CState_Lobby_Destroyer_Neutral_Idle::Exit_State()
{
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = 0;
}

CState_Lobby_Destroyer_Neutral_Idle* CState_Lobby_Destroyer_Neutral_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Lobby_Destroyer_Neutral_Idle* pInstance = new CState_Lobby_Destroyer_Neutral_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Lobby_Destroyer_Neutral_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Lobby_Destroyer_Neutral_Idle::Free()
{
    __super::Free();
}

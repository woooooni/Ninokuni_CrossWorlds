#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Crouch_Idle.h"

CState_Character_Neutral_Crouch_Idle::CState_Character_Neutral_Crouch_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Crouch_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Crouch_Idle::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Character_Neutral_Crouch_Idle::Tick_State(_float fTimeDelta)
{
    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[1];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    if (m_iCurrAnimIndex == m_AnimIndices[1] && false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
    {
        m_fAccIdleMotion = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[0];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    __super::Crouch_Idle_Input(fTimeDelta);
}


void CState_Character_Neutral_Crouch_Idle::Exit_State()
{
    
}


CState_Character_Neutral_Crouch_Idle* CState_Character_Neutral_Crouch_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Crouch_Idle* pInstance = new CState_Character_Neutral_Crouch_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Crouch_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Crouch_Idle::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Kick.h"

CState_Character_Neutral_Kick::CState_Character_Neutral_Kick(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Kick::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Kick::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_fAccIdleMotion = 0.f;
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Neutral_Kick::Tick_State(_float fTimeDelta)
{
    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_pModelCom->Set_Animation(m_AnimIndices[GI->RandomInt(1, 2)]);
    }

}

void CState_Character_Neutral_Kick::Exit_State()
{
    m_fAccIdleMotion = 0.f;
}



CState_Character_Neutral_Kick* CState_Character_Neutral_Kick::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Kick* pInstance = new CState_Character_Neutral_Kick(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Kick");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Kick::Free()
{
    __super::Free();
}

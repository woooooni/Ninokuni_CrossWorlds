#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Crouch_Idle.h"

CState_SwordMan_Neutral_Crouch_Idle::CState_SwordMan_Neutral_Crouch_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Crouch_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Neutral_Crouch_Idle::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Crouch_Idle::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);

    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_pModelCom->Set_Animation(m_AnimIndices[1]);
    }
}

void CState_SwordMan_Neutral_Crouch_Idle::Exit_State()
{

}

void CState_SwordMan_Neutral_Crouch_Idle::Input(_float fTimeDelta)
{
    if (KEY_HOLD(KEY::W) || KEY_TAP(KEY::A) || KEY_TAP(KEY::S) || KEY_TAP(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_CROUCH_MOVE);

    if (KEY_TAP(KEY::C))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
    
}

CState_SwordMan_Neutral_Crouch_Idle* CState_SwordMan_Neutral_Crouch_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Crouch_Idle* pInstance = new CState_SwordMan_Neutral_Crouch_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Crouch_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Crouch_Idle::Free()
{
    __super::Free();
}

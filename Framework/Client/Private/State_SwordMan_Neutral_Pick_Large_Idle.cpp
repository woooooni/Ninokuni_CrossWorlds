#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Large_Idle.h"

CState_SwordMan_Neutral_Pick_Large_Idle::CState_SwordMan_Neutral_Pick_Large_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Large_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Large_Idle::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Large_Idle::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);

    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_pModelCom->Set_Animation(m_AnimIndices[1]);
    }
}

void CState_SwordMan_Neutral_Pick_Large_Idle::Exit_State()
{

}

void CState_SwordMan_Neutral_Pick_Large_Idle::Input(_float fTimeDelta)
{
    if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_CROUCH_MOVE);

    if (KEY_TAP(KEY::C))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
    
}

CState_SwordMan_Neutral_Pick_Large_Idle* CState_SwordMan_Neutral_Pick_Large_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Large_Idle* pInstance = new CState_SwordMan_Neutral_Pick_Large_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Large_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Large_Idle::Free()
{
    __super::Free();
}

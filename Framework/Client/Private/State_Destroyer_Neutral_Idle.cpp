#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Neutral_Idle.h"

CState_Destroyer_Neutral_Idle::CState_Destroyer_Neutral_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Neutral_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Neutral_Idle::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Destroyer_Neutral_Idle::Tick_State(_float fTimeDelta)
{
    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 2)];

        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    if (m_iCurrAnimIndex != m_AnimIndices[0] && false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
    {
        m_fAccIdleMotion = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[0];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    Input(fTimeDelta);


}

void CState_Destroyer_Neutral_Idle::Exit_State()
{
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = 0;
}

void CState_Destroyer_Neutral_Idle::Input(_float fTimeDelta)
{
    if (true == Skill_Input(fTimeDelta))
        return;

    if (KEY_TAP(KEY::CTRL))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
        return;
    }

    if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_WALK);

    if (KEY_HOLD(KEY::RBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
        return;
    }

    if (KEY_TAP(KEY::LBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
        return;
    }
    
    if (KEY_TAP(KEY::SPACE))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_JUMP);

    if (KEY_TAP(KEY::C))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_CROUCH_IDLE);
}

CState_Destroyer_Neutral_Idle* CState_Destroyer_Neutral_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Neutral_Idle* pInstance = new CState_Destroyer_Neutral_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_Neutral_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Neutral_Idle::Free()
{
    __super::Free();
}

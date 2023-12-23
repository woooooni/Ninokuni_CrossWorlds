#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Guard.h"

CState_SwordMan_Battle_Guard::CState_SwordMan_Battle_Guard(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Guard::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Battle_Guard::Enter_State(void* pArg)
{
    m_fAccIdleMotion = 0.f;
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Battle_Guard::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);

    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_pModelCom->Set_Animation(m_AnimIndices[GI->RandomInt(1, 2)]);
    }
}

void CState_SwordMan_Battle_Guard::Exit_State()
{
    m_fAccIdleMotion = 0.f;
}

void CState_SwordMan_Battle_Guard::Input(_float fTimeDelta)
{
    if (KEY_HOLD(KEY::W) || KEY_TAP(KEY::A) || KEY_TAP(KEY::S) || KEY_TAP(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_WALK);
    
    if (KEY_TAP(KEY::SPACE))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_JUMP);

    if (KEY_TAP(KEY::C))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_CROUCH_IDLE);
}

CState_SwordMan_Battle_Guard* CState_SwordMan_Battle_Guard::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Guard* pInstance = new CState_SwordMan_Battle_Guard(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Guard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Guard::Free()
{
    __super::Free();
}

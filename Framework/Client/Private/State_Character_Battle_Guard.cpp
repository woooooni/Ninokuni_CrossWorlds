#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Battle_Guard.h"

CState_Character_Battle_Guard::CState_Character_Battle_Guard(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Battle_Guard::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Battle_Guard::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0], MIN_TWEEN_DURATION);
}

void CState_Character_Battle_Guard::Tick_State(_float fTimeDelta)
{
    if (KEY_NONE(KEY::RBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
        return;
    }
}

void CState_Character_Battle_Guard::Exit_State()
{
    
}


CState_Character_Battle_Guard* CState_Character_Battle_Guard::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Battle_Guard* pInstance = new CState_Character_Battle_Guard(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Battle_Guard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Battle_Guard::Free()
{
    __super::Free();
}

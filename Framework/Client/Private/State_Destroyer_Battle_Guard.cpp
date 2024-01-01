#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Battle_Guard.h"

CState_Destroyer_Battle_Guard::CState_Destroyer_Battle_Guard(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Battle_Guard::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Battle_Guard::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_Battle_Guard::Tick_State(_float fTimeDelta)
{
    m_pCharacter->Appear_Weapon();
    Input(fTimeDelta);
}

void CState_Destroyer_Battle_Guard::Exit_State()
{
    
}

void CState_Destroyer_Battle_Guard::Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::CTRL))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
        return;
    }

    if (KEY_NONE(KEY::RBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::BATTLE_IDLE);
        return;
    }
        
}

CState_Destroyer_Battle_Guard* CState_Destroyer_Battle_Guard::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Battle_Guard* pInstance = new CState_Destroyer_Battle_Guard(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_Battle_Guard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Battle_Guard::Free()
{
    __super::Free();
}

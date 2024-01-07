#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Revive.h"

CState_Character_Revive::CState_Character_Revive(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Revive::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Revive::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Revive::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Character_Revive::Exit_State()
{

}



CState_Character_Revive* CState_Character_Revive::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Revive* pInstance = new CState_Character_Revive(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Revive");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Revive::Free()
{
    __super::Free();
}

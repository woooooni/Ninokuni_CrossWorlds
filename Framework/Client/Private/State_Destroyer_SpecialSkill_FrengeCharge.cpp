#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_SpecialSkill_FrengeCharge.h"

CState_Destroyer_SpecialSkill_FrengeCharge::CState_Destroyer_SpecialSkill_FrengeCharge(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_SpecialSkill_FrengeCharge::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_SpecialSkill_FrengeCharge::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_SpecialSkill_FrengeCharge::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_SpecialSkill_FrengeCharge::Exit_State()
{
    
}

CState_Destroyer_SpecialSkill_FrengeCharge* CState_Destroyer_SpecialSkill_FrengeCharge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_SpecialSkill_FrengeCharge* pInstance = new CState_Destroyer_SpecialSkill_FrengeCharge(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_SpecialSkill_FrengeCharge::Free()
{
    __super::Free();
}
#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_SpecialSkill_SwordTempest.h"

CState_SwordMan_SpecialSkill_SwordTempest::CState_SwordMan_SpecialSkill_SwordTempest(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_SpecialSkill_SwordTempest::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_SpecialSkill_SwordTempest::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_SpecialSkill_SwordTempest::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_SpecialSkill_SwordTempest::Exit_State()
{
    
}

CState_SwordMan_SpecialSkill_SwordTempest* CState_SwordMan_SpecialSkill_SwordTempest::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_SpecialSkill_SwordTempest* pInstance = new CState_SwordMan_SpecialSkill_SwordTempest(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_SpecialSkill_SwordTempest::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "State_Character.h"
#include "Character.h"

CState_Character::CState_Character(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}



HRESULT CState_Character::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);
	m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pCharacter)
		return E_FAIL;

	return S_OK;
}

_bool CState_Character::Skill_Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::R))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::SKILL_BURST);
        return true;
    }


    if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::NUM_1))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::SKILL_SPECIAL_0);
        return true;
    }

    if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::NUM_2))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::SKILL_SPECIAL_1);
        return true;
    }

    if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::NUM_3))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::SKILL_SPECIAL_2);
        return true;
    }

    if (KEY_TAP(KEY::NUM_1))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::CLASS_SKILL_0);
        return true;
    }

    if (KEY_TAP(KEY::NUM_2))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::CLASS_SKILL_1);
        return true;
    }

    if (KEY_TAP(KEY::NUM_3))
    {
        // 임시
        m_pStateMachineCom->Change_State(CCharacter::CLASS_SKILL_2);
        return true;
    }


    return false;
}

void CState_Character::Free()
{
	__super::Free();
}

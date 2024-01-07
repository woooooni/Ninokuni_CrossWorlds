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

void CState_Character::Free()
{
	__super::Free();
}

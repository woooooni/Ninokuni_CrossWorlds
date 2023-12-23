#include "stdafx.h"
#include "GameInstance.h"
#include "State_Character.h"


CState_Character::CState_Character(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}



HRESULT CState_Character::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);
	return S_OK;
}

void CState_Character::Free()
{
	__super::Free();
}

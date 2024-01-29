#include "stdafx.h"
#include "State_CurlingGame_Move_Character.h"

#include "GameInstance.h"

CState_CurlingGame_Move_Character::CState_CurlingGame_Move_Character(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Move_Character::Initialize()
{
	return S_OK;
}

void CState_CurlingGame_Move_Character::Enter_State(void* pArg)
{
}

void CState_CurlingGame_Move_Character::Tick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Move_Character::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Move_Character::Exit_State()
{
}

HRESULT CState_CurlingGame_Move_Character::Render()
{
	return S_OK;
}

CState_CurlingGame_Move_Character* CState_CurlingGame_Move_Character::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Move_Character* pInstance = new CState_CurlingGame_Move_Character(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Move_Character");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Move_Character::Free()
{
	__super::Free();
}


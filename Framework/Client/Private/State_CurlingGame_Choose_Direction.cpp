#include "stdafx.h"
#include "State_CurlingGame_Choose_Direction.h"

#include "GameInstance.h"

CState_CurlingGame_Choose_Direction::CState_CurlingGame_Choose_Direction(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Choose_Direction::Initialize()
{
	return S_OK;
}

void CState_CurlingGame_Choose_Direction::Enter_State(void* pArg)
{
}

void CState_CurlingGame_Choose_Direction::Tick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Choose_Direction::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Choose_Direction::Exit_State()
{
}

HRESULT CState_CurlingGame_Choose_Direction::Render()
{
	return S_OK;
}

CState_CurlingGame_Choose_Direction* CState_CurlingGame_Choose_Direction::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Choose_Direction* pInstance = new CState_CurlingGame_Choose_Direction(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Choose_Direction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Choose_Direction::Free()
{
	__super::Free();
}


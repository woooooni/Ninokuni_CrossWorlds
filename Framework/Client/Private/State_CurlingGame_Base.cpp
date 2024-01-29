#include "stdafx.h"
#include "State_CurlingGame_Base.h"

#include "GameInstance.h"

CState_CurlingGame_Base::CState_CurlingGame_Base(CManager_StateMachine* pStateMachine)
	: CManager_State(pStateMachine)
{
}

HRESULT CState_CurlingGame_Base::Initialize()
{
	return S_OK;
}

void CState_CurlingGame_Base::Enter_State(void* pArg)
{
}

void CState_CurlingGame_Base::Tick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Base::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Base::Exit_State()
{
}

HRESULT CState_CurlingGame_Base::Render()
{
	return S_OK;
}

void CState_CurlingGame_Base::Free()
{
	__super::Free();
}


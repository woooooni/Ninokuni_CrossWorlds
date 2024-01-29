#include "stdafx.h"
#include "State_CurlingGame_Intro.h"

#include "GameInstance.h"

CState_CurlingGame_Intro::CState_CurlingGame_Intro(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Intro::Initialize()
{
	return S_OK;
}

void CState_CurlingGame_Intro::Enter_State(void* pArg)
{
}

void CState_CurlingGame_Intro::Tick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Intro::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Intro::Exit_State()
{
}

HRESULT CState_CurlingGame_Intro::Render()
{
	return S_OK;
}

CState_CurlingGame_Intro* CState_CurlingGame_Intro::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Intro* pInstance = new CState_CurlingGame_Intro(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Intro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Intro::Free()
{
	__super::Free();
}


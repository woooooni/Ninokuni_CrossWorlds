#include "stdafx.h"
#include "State_CurlingGame_Adjust_Intensity.h"

#include "GameInstance.h"

CState_CurlingGame_Adjust_Intensity::CState_CurlingGame_Adjust_Intensity(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Adjust_Intensity::Initialize()
{
	return S_OK;
}

void CState_CurlingGame_Adjust_Intensity::Enter_State(void* pArg)
{
}

void CState_CurlingGame_Adjust_Intensity::Tick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Adjust_Intensity::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Adjust_Intensity::Exit_State()
{
}

HRESULT CState_CurlingGame_Adjust_Intensity::Render()
{
	return S_OK;
}

CState_CurlingGame_Adjust_Intensity* CState_CurlingGame_Adjust_Intensity::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Adjust_Intensity* pInstance = new CState_CurlingGame_Adjust_Intensity(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Adjust_Intensity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Adjust_Intensity::Free()
{
	__super::Free();
}


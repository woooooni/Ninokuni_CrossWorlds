#include "stdafx.h"
#include "State_CurlingGame_Ending.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"


CState_CurlingGame_Ending::CState_CurlingGame_Ending(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Ending::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Ending::Enter_State(void* pArg)
{

}

void CState_CurlingGame_Ending::Tick_State(const _float& fTimeDelta)
{

}

void CState_CurlingGame_Ending::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Ending::Exit_State()
{
}

HRESULT CState_CurlingGame_Ending::Render()
{
	return S_OK;
}

CState_CurlingGame_Ending* CState_CurlingGame_Ending::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Ending* pInstance = new CState_CurlingGame_Ending(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Ending::Free()
{
	__super::Free();
}


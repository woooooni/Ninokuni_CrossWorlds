#include "stdafx.h"
#include "State_CurlingGame_Launch_Stone.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"

CState_CurlingGame_Launch_Stone::CState_CurlingGame_Launch_Stone(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Launch_Stone::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Launch_Stone::Enter_State(void* pArg)
{
	/* 댐핑 시작 */
	{
		CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr == pCurlingCam)
			return;

		pCurlingCam->Start_Damping();
	}
}

void CState_CurlingGame_Launch_Stone::Tick_State(const _float& fTimeDelta)
{

	// 댐핑 종료 -> fov 럴프 종료 여부
	// 점수 계산
	// 타겟 변경 및 세팅
	// 카메라 복귀 후 다시 턴 변경 
}

void CState_CurlingGame_Launch_Stone::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Launch_Stone::Exit_State()
{
	CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	if (nullptr == pCurlingCam)
		return;
}

HRESULT CState_CurlingGame_Launch_Stone::Render()
{
	return S_OK;
}

CState_CurlingGame_Launch_Stone* CState_CurlingGame_Launch_Stone::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Launch_Stone* pInstance = new CState_CurlingGame_Launch_Stone(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Launch_Stone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Launch_Stone::Free()
{
	__super::Free();
}


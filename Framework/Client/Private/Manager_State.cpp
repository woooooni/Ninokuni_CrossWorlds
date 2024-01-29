#include "stdafx.h"
#include "..\Public\Manager_State.h"

#include "GameInstance.h"

#include "Manager_StateMachine.h"

CManager_State::CManager_State(CManager_StateMachine* pStateMachine)
	: m_pManager_StateMachine(pStateMachine)
{
}

HRESULT CManager_State::Initialize()
{
	return S_OK;
}


void CManager_State::Free()
{
	__super::Free();
}

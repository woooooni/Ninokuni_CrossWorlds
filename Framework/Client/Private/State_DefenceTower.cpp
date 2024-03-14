#include "stdafx.h"
#include "GameInstance.h"
#include "State_DefenceTower.h"
#include "Defence_Tower.h"
#include "UI_Manager.h"
#include "UI_Dialog_Window.h"
#include "Game_Manager.h"


CState_DefenceTower::CState_DefenceTower(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}



HRESULT CState_DefenceTower::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	m_pTower = dynamic_cast<CDefence_Tower*>(m_pStateMachineCom->Get_Owner());

	if (nullptr == m_pTower)
		return E_FAIL;

	return S_OK;
}

void CState_DefenceTower::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy.h"
#include "Grandprix_Enemy.h"

CState_Enemy::CState_Enemy(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}

HRESULT CState_Enemy::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);
	m_pEnemy = dynamic_cast<CGrandprix_Enemy*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pEnemy)
		return E_FAIL;

	return S_OK;
}

void CState_Enemy::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "NpcCriminalState_LockPick.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcCriminalState_LockPick::CNpcCriminalState_LockPick(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcCriminalState_LockPick::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcCriminalState_LockPick::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcCriminalState_LockPick::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcCriminalState_LockPick::Exit_State()
{
}

CNpcCriminalState_LockPick* CNpcCriminalState_LockPick::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcCriminalState_LockPick* pInstance = new CNpcCriminalState_LockPick(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcCriminalState_LockPick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcCriminalState_LockPick::Free()
{
	__super::Free();
}


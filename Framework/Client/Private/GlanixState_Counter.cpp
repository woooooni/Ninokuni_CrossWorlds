#include "stdafx.h"
#include "GlanixState_Counter.h"

#include "Glanix.h"

CGlanixState_Counter::CGlanixState_Counter(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Counter::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fCounterTime = 5.f;

	return S_OK;
}

void CGlanixState_Counter::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;
}

void CGlanixState_Counter::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fCounterTime)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
}

void CGlanixState_Counter::Exit_State()
{
}

CGlanixState_Counter* CGlanixState_Counter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Counter* pInstance = new CGlanixState_Counter(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Counter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Counter::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_Stun.h"

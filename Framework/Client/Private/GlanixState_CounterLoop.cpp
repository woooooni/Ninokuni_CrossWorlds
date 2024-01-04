#include "stdafx.h"
#include "GlanixState_CounterLoop.h"

#include "Glanix.h"

CGlanixState_CounterLoop::CGlanixState_CounterLoop(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_CounterLoop::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fCounterTime = 5.f;

	return S_OK;
}

void CGlanixState_CounterLoop::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_CounterLoop"));
	m_fTime = 0.f;
}

void CGlanixState_CounterLoop::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fCounterTime)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COUNTEREND);
}

void CGlanixState_CounterLoop::Exit_State()
{
}

CGlanixState_CounterLoop* CGlanixState_CounterLoop::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_CounterLoop* pInstance = new CGlanixState_CounterLoop(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_CounterLoop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_CounterLoop::Free()
{
	__super::Free();
}


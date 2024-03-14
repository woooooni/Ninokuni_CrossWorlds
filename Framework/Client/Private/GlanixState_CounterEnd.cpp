#include "stdafx.h"
#include "GlanixState_CounterEnd.h"

#include "Glanix.h"

CGlanixState_CounterEnd::CGlanixState_CounterEnd(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_CounterEnd::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_CounterEnd::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_CounterOut"));
}

void CGlanixState_CounterEnd::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_CounterEnd::Exit_State()
{
}

CGlanixState_CounterEnd* CGlanixState_CounterEnd::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_CounterEnd* pInstance = new CGlanixState_CounterEnd(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_CounterEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_CounterEnd::Free()
{
	__super::Free();
}

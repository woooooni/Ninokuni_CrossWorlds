#include "stdafx.h"
#include "GlanixState_CounterStart.h"

#include "Glanix.h"

CGlanixState_CounterStart::CGlanixState_CounterStart(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_CounterStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_CounterStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_CounterEnter"));
}

void CGlanixState_CounterStart::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COUNTERLOOP);
	}
}

void CGlanixState_CounterStart::Exit_State()
{
}

CGlanixState_CounterStart* CGlanixState_CounterStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_CounterStart* pInstance = new CGlanixState_CounterStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_CounterStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_CounterStart::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_CounterStart.h"

#include "Glanix.h"

#include "GameInstance.h"

CGlanixState_CounterStart::CGlanixState_CounterStart(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_CounterStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fSlowTime = 0.05f;
	
	return S_OK;
}

void CGlanixState_CounterStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_CounterEnter"), MIN_TWEEN_DURATION);
	GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 0.1f);
	m_fTime = 0.f;
	m_bIsSlow = true;
}

void CGlanixState_CounterStart::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_bIsSlow)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= m_fSlowTime)
		{
			m_bIsSlow = false;
			GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 1.f);
		}
	}



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

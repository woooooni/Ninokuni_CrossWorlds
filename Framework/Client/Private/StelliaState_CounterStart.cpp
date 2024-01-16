#include "stdafx.h"
#include "StelliaState_CounterStart.h"

#include "Stellia.h"

#include "GameInstance.h"

CStelliaState_CounterStart::CStelliaState_CounterStart(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_CounterStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fSlowTime = 0.05f;

	return S_OK;
}

void CStelliaState_CounterStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_CounterStart"), MIN_TWEEN_DURATION);
	m_fTime = 0.f;

	m_bIsTimeSlep = false;
}

void CStelliaState_CounterStart::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (!m_bIsTimeSlep && m_pModelCom->Get_CurrAnimationFrame() == 5)
	{
		GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 0.1f);
		m_bIsTimeSlep = true;
		m_bIsSlow = true;
	}

	if (m_bIsTimeSlep)
	{
		if (m_bIsSlow)
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= m_fSlowTime)
			{
				m_bIsTimeSlep = false;
				m_bIsSlow = false;
				GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 1.f);
			}
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COUNTERLOOP);
	}
}

void CStelliaState_CounterStart::Exit_State()
{
}

CStelliaState_CounterStart* CStelliaState_CounterStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_CounterStart* pInstance = new CStelliaState_CounterStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_CounterStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_CounterStart::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "StelliaState_Rage3Damage.h"

#include "Stellia.h"

#include "GameInstance.h"

CStelliaState_Rage3Damage::CStelliaState_Rage3Damage(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Damage::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fSlowTime = 0.1f;
	m_fStunTime = 0.1f;

	return S_OK;
}

void CStelliaState_Rage3Damage::Enter_State(void* pArg)
{
	m_pStellia->Set_StelliaHit(true);

	// m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_CounterStart"), MIN_TWEEN_DURATION);
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_InstantTurn"), MIN_TWEEN_DURATION);
	m_fTime = 0.f;

	m_bIsTimeSlep = false;
}

void CStelliaState_Rage3Damage::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 8 && m_pModelCom->Get_CurrAnimationFrame() <= 29)
	{
		m_pTransformCom->Move(-m_pTransformCom->Get_Look(), m_fSprintSpeed, fTimeDelta);
	}

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
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3ESCAPE);
	}
}

void CStelliaState_Rage3Damage::Exit_State()
{
}

CStelliaState_Rage3Damage* CStelliaState_Rage3Damage::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Damage* pInstance = new CStelliaState_Rage3Damage(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Damage::Free()
{
	__super::Free();
}

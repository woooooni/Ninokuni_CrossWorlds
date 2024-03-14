#include "stdafx.h"
#include "DMWitchState_Rage.h"

#include "DMWitch.h"

CDMWitchState_Rage::CDMWitchState_Rage(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Rage::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Rage::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_BossSkillRage01"));
}

void CDMWitchState_Rage::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_Rage::Exit_State()
{
}

CDMWitchState_Rage* CDMWitchState_Rage::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Rage* pInstance = new CDMWitchState_Rage(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Rage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Rage::Free()
{
	__super::Free();
}


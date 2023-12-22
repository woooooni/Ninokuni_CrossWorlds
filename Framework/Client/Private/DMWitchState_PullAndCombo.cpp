#include "stdafx.h"
#include "DMWitchState_PullAndCombo.h"

#include "DMWitch.h"

CDMWitchState_PullAndCombo::CDMWitchState_PullAndCombo(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_PullAndCombo::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_PullAndCombo::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill05"));
}

void CDMWitchState_PullAndCombo::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_PullAndCombo::Exit_State()
{
}

CDMWitchState_PullAndCombo* CDMWitchState_PullAndCombo::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_PullAndCombo* pInstance = new CDMWitchState_PullAndCombo(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_PullAndCombo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_PullAndCombo::Free()
{
	__super::Free();
}


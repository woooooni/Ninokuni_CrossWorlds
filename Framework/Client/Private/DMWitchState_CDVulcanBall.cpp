#include "stdafx.h"
#include "DMWitchState_CDVulcanBall.h"

#include "DMWitch.h"

CDMWitchState_CDVulcanBall::CDMWitchState_CDVulcanBall(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_CDVulcanBall::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_CDVulcanBall::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill06"));
}

void CDMWitchState_CDVulcanBall::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_CHILDDREAM_END);
	}
}

void CDMWitchState_CDVulcanBall::Exit_State()
{
}

CDMWitchState_CDVulcanBall* CDMWitchState_CDVulcanBall::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_CDVulcanBall* pInstance = new CDMWitchState_CDVulcanBall(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_CDVulcanBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_CDVulcanBall::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "DMWitchState_VulcanBall.h"

#include "DMWitch.h"

CDMWitchState_VulcanBall::CDMWitchState_VulcanBall(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_VulcanBall::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_VulcanBall::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill06"));
}

void CDMWitchState_VulcanBall::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_VulcanBall::Exit_State()
{
}

CDMWitchState_VulcanBall* CDMWitchState_VulcanBall::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_VulcanBall* pInstance = new CDMWitchState_VulcanBall(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_VulcanBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_VulcanBall::Free()
{
	__super::Free();
}


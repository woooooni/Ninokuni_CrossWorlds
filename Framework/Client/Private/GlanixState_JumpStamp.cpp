#include "stdafx.h"
#include "GlanixState_JumpStamp.h"

#include "Glanix.h"

CGlanixState_JumpStamp::CGlanixState_JumpStamp(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_JumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_JumpStamp::Enter_State(void* pArg)
{
	// 플레이어의 위치와 기안티 위치 사이의 거리를 구하기.
	// 점프와 스피드 러프 두 개가 필요할 듯. 
	
	//m_tJumpLerp.Start(1.f, 2.f, 1.f, LERP_MODE::EASE_IN);
	//m_tJumpLerp.fCurValue = m_tJumpLerp.fStartValue;

	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill03"));
}

void CGlanixState_JumpStamp::Tick_State(_float fTimeDelta)
{
	//m_tJumpLerp.Update(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//_float fWaitTime = 2.f;
		//m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_JumpStamp::Exit_State()
{
}

CGlanixState_JumpStamp* CGlanixState_JumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_JumpStamp* pInstance = new CGlanixState_JumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_JumpStamp::Free()
{
	__super::Free();
}


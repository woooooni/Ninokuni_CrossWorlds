#include "stdafx.h"
#include "GlanixState_SnowBall.h"

#include "Glanix.h"

CGlanixState_SnowBall::CGlanixState_SnowBall(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_SnowBall::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_SnowBall::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill08"));
}

void CGlanixState_SnowBall::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
}

void CGlanixState_SnowBall::Exit_State()
{
}

CGlanixState_SnowBall* CGlanixState_SnowBall::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_SnowBall* pInstance = new CGlanixState_SnowBall(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_SnowBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_SnowBall::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_SnowBall.h"

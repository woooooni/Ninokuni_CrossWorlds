#include "stdafx.h"
#include "GlanixState_QuadBlow.h"

#include "Glanix.h"

CGlanixState_QuadBlow::CGlanixState_QuadBlow(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_QuadBlow::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_QuadBlow::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill02_New"));
}

void CGlanixState_QuadBlow::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//_float fWaitTime = 2.f;
		//m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_QuadBlow::Exit_State()
{
}

CGlanixState_QuadBlow* CGlanixState_QuadBlow::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_QuadBlow* pInstance = new CGlanixState_QuadBlow(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_QuadBlow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_QuadBlow::Free()
{
	__super::Free();
}


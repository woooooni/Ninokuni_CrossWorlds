#include "stdafx.h"
#include "GlanixState_Attack2.h"

#include "Glanix.h"

CGlanixState_Attack2::CGlanixState_Attack2(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Attack2::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Attack2::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Attack02"));
}

void CGlanixState_Attack2::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//_float fWaitTime = 2.f;
		//m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_Attack2::Exit_State()
{
}

CGlanixState_Attack2* CGlanixState_Attack2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Attack2* pInstance = new CGlanixState_Attack2(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Attack2::Free()
{
	__super::Free();
}


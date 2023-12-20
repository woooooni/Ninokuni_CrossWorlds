#include "stdafx.h"
#include "GlanixState_Attack1.h"

#include "Glanix.h"

CGlanixState_Attack1::CGlanixState_Attack1(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Attack1::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Attack1::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Attack01"));
}

void CGlanixState_Attack1::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//_float fWaitTime = 2.f;
		//m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_Attack1::Exit_State()
{
}

CGlanixState_Attack1* CGlanixState_Attack1::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Attack1* pInstance = new CGlanixState_Attack1(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Attack1::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GlanixState_RageCrash.h"

#include "Glanix.h"

CGlanixState_RageCrash::CGlanixState_RageCrash(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageCrash::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageCrash::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Crash"));
}

void CGlanixState_RageCrash::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageCrash::Exit_State()
{
}

CGlanixState_RageCrash* CGlanixState_RageCrash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageCrash* pInstance = new CGlanixState_RageCrash(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageCrash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageCrash::Free()
{
	__super::Free();
}

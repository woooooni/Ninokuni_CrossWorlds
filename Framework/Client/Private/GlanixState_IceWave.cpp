#include "stdafx.h"
#include "GlanixState_IceWave.h"

#include "Glanix.h"

CGlanixState_IceWave::CGlanixState_IceWave(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_IceWave::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_IceWave::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill06"));
}

void CGlanixState_IceWave::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//_float fWaitTime = 2.f;
		//m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_IceWave::Exit_State()
{
}

CGlanixState_IceWave* CGlanixState_IceWave::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_IceWave* pInstance = new CGlanixState_IceWave(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_IceWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_IceWave::Free()
{
	__super::Free();
}


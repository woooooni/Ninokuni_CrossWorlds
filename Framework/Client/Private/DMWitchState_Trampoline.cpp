#include "stdafx.h"
#include "DMWitchState_Trampoline.h"

#include "DMWitch.h"

CDMWitchState_Trampoline::CDMWitchState_Trampoline(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Trampoline::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Trampoline::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill07"));
}

void CDMWitchState_Trampoline::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_Trampoline::Exit_State()
{
}

CDMWitchState_Trampoline* CDMWitchState_Trampoline::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Trampoline* pInstance = new CDMWitchState_Trampoline(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Trampoline");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Trampoline::Free()
{
	__super::Free();
}


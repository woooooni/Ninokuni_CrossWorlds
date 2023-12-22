#include "stdafx.h"
#include "DMWitchState_CDEnd.h"

#include "DMWitch.h"

CDMWitchState_CDEnd::CDMWitchState_CDEnd(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_CDEnd::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_CDEnd::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill022"));
}

void CDMWitchState_CDEnd::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_CDEnd::Exit_State()
{
}

CDMWitchState_CDEnd* CDMWitchState_CDEnd::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_CDEnd* pInstance = new CDMWitchState_CDEnd(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_CDEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_CDEnd::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "DMWitchState_CDStart.h"

#include "DMWitch.h"

CDMWitchState_CDStart::CDMWitchState_CDStart(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_CDStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_CDStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill021"));
}

void CDMWitchState_CDStart::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_CHILDDREAM_VULCANBALL);
	}
}

void CDMWitchState_CDStart::Exit_State()
{
}

CDMWitchState_CDStart* CDMWitchState_CDStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_CDStart* pInstance = new CDMWitchState_CDStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_CDStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_CDStart::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "DMWitchState_CollFile.h"

#include "DMWitch.h"

CDMWitchState_CollFire::CDMWitchState_CollFire(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_CollFire::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_CollFire::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill04"));
}

void CDMWitchState_CollFire::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_CollFire::Exit_State()
{
}

CDMWitchState_CollFire* CDMWitchState_CollFire::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_CollFire* pInstance = new CDMWitchState_CollFire(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_CollFire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_CollFire::Free()
{
	__super::Free();
}


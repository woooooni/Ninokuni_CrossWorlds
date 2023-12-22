#include "stdafx.h"
#include "DMWitchState_Attack1.h"

#include "DMWitch.h"

CDMWitchState_Attack1::CDMWitchState_Attack1(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Attack1::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Attack1::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack01"));
}

void CDMWitchState_Attack1::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_Attack1::Exit_State()
{
}

CDMWitchState_Attack1* CDMWitchState_Attack1::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Attack1* pInstance = new CDMWitchState_Attack1(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Attack1::Free()
{
	__super::Free();
}


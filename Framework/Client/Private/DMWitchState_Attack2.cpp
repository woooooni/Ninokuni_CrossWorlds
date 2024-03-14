#include "stdafx.h"
#include "DMWitchState_Attack2.h"

#include "DMWitch.h"

CDMWitchState_Attack2::CDMWitchState_Attack2(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Attack2::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Attack2::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack02"));
}

void CDMWitchState_Attack2::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_Attack2::Exit_State()
{
}

CDMWitchState_Attack2* CDMWitchState_Attack2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Attack2* pInstance = new CDMWitchState_Attack2(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Attack2::Free()
{
	__super::Free();
}


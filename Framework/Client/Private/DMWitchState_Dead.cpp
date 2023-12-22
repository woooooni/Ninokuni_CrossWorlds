#include "stdafx.h"
#include "DMWitchState_Dead.h"

#include "DMWitch.h"

CDMWitchState_Dead::CDMWitchState_Dead(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Death"));
}

void CDMWitchState_Dead::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pWitch->Reserve_Dead(true);
	}
}

void CDMWitchState_Dead::Exit_State()
{
}

CDMWitchState_Dead* CDMWitchState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Dead* pInstance = new CDMWitchState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Dead::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "DMWitchState_QuadBomb.h"

#include "DMWitch.h"

CDMWitchState_QuadBomb::CDMWitchState_QuadBomb(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_QuadBomb::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_QuadBomb::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill01"));
}

void CDMWitchState_QuadBomb::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_QuadBomb::Exit_State()
{
}

CDMWitchState_QuadBomb* CDMWitchState_QuadBomb::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_QuadBomb* pInstance = new CDMWitchState_QuadBomb(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_QuadBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_QuadBomb::Free()
{
	__super::Free();
}


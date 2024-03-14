#include "stdafx.h"
#include "DMWitchState_Spawn.h"

#include "GameInstance.h"

#include "DMWitch.h"

CDMWitchState_Spawn::CDMWitchState_Spawn(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Spawn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Spawn::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_CSDreamersMazeWitch01"));

	if (m_pPlayer != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());
}

void CDMWitchState_Spawn::Tick_State(_float fTimeDelta)
{
	// player가 없으면 그냥 굳어버리게.
	if (m_pPlayer != nullptr)
	{
		if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		{
			m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
		}
	}
}

void CDMWitchState_Spawn::Exit_State()
{
}

CDMWitchState_Spawn* CDMWitchState_Spawn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Spawn* pInstance = new CDMWitchState_Spawn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Spawn::Free()
{
	__super::Free();
}


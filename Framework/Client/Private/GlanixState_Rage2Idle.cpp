#include "stdafx.h"
#include "GlanixState_Rage2Idle.h"

#include "Glanix.h"

CGlanixState_Rage2Idle::CGlanixState_Rage2Idle(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iMaxCount = 6;

	return S_OK;
}

void CGlanixState_Rage2Idle::Enter_State(void* pArg)
{
	if (pArg != nullptr)
		m_iWaveCount = *(_int*)pArg;

	if (m_iWaveCount < m_iMaxCount)
		m_fWaitTime = 2.f;
	else
	{
		m_iWaveCount = 0;
		m_fWaitTime = 4.f;
	}

	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;
}

void CGlanixState_Rage2Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fTime += fTimeDelta;

	if (m_fTime >= m_fWaitTime)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2WAVE);

}

void CGlanixState_Rage2Idle::Exit_State()
{
}

CGlanixState_Rage2Idle* CGlanixState_Rage2Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Idle* pInstance = new CGlanixState_Rage2Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Idle::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GlanixState_CombatIdle.h"

#include "Glanix.h"

CGlanixState_CombatIdle::CGlanixState_CombatIdle(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_CombatIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_CombatIdle::Enter_State(void* pArg)
{
	if (pArg != nullptr)
		m_fWaitTime = *(_float*)pArg;

	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));

	m_fTime = 0.f;
}

void CGlanixState_CombatIdle::Tick_State(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fWaitTime)
	{
		if (m_pGlanix->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
		else
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_CHASE);
	}
}

void CGlanixState_CombatIdle::Exit_State()
{
}

CGlanixState_CombatIdle* CGlanixState_CombatIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_CombatIdle* pInstance = new CGlanixState_CombatIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_CombatIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_CombatIdle::Free()
{
	__super::Free();
}


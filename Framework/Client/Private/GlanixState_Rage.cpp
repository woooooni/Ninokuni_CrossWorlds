#include "stdafx.h"
#include "GlanixState_Rage.h"

#include "Glanix.h"

CGlanixState_Rage::CGlanixState_Rage(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Rage::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
}

void CGlanixState_Rage::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_Rage::Exit_State()
{
}

CGlanixState_Rage* CGlanixState_Rage::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage* pInstance = new CGlanixState_Rage(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_Rage.h"

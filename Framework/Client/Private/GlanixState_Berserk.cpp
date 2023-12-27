#include "stdafx.h"
#include "GlanixState_Berserk.h"

#include "Glanix.h"

CGlanixState_Berserk::CGlanixState_Berserk(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Berserk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Berserk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
}

void CGlanixState_Berserk::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_Berserk::Exit_State()
{
}

CGlanixState_Berserk* CGlanixState_Berserk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Berserk* pInstance = new CGlanixState_Berserk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Berserk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Berserk::Free()
{
	__super::Free();
}

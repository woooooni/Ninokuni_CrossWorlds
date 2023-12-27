#include "stdafx.h"
#include "GlanixState_RageStart.h"

#include "Glanix.h"

CGlanixState_RageStart::CGlanixState_RageStart(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
}

void CGlanixState_RageStart::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageStart::Exit_State()
{
}

CGlanixState_RageStart* CGlanixState_RageStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStart* pInstance = new CGlanixState_RageStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStart::Free()
{
	__super::Free();
}

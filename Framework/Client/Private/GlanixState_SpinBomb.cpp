#include "stdafx.h"
#include "GlanixState_SpinBomb.h"

#include "Glanix.h"

CGlanixState_SpinBomb::CGlanixState_SpinBomb(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_SpinBomb::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_SpinBomb::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill01"));
}

void CGlanixState_SpinBomb::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_SpinBomb::Exit_State()
{
}

CGlanixState_SpinBomb* CGlanixState_SpinBomb::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_SpinBomb* pInstance = new CGlanixState_SpinBomb(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_SpinBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_SpinBomb::Free()
{
	__super::Free();
}


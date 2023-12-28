#include "stdafx.h"
#include "GlanixState_Stun.h"

#include "Glanix.h"

CGlanixState_Stun::CGlanixState_Stun(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Stun::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fStunTime = 10.f;

	return S_OK;
}

void CGlanixState_Stun::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;
}

void CGlanixState_Stun::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fStunTime)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
}

void CGlanixState_Stun::Exit_State()
{
}

CGlanixState_Stun* CGlanixState_Stun::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Stun* pInstance = new CGlanixState_Stun(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Stun::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_Stun.h"

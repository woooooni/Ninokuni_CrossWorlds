#include "stdafx.h"
#include "GlanixState_IntroRoar.h"

#include "Glanix.h"

CGlanixState_IntroRoar::CGlanixState_IntroRoar(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_IntroRoar::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_IntroRoar::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
	m_pTransformCom->LookAt_ForLandObject(m_pGlanix->Get_OriginPos());
}

void CGlanixState_IntroRoar::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_INTRO_JUMP);
	}
}

void CGlanixState_IntroRoar::Exit_State()
{
}

CGlanixState_IntroRoar* CGlanixState_IntroRoar::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_IntroRoar* pInstance = new CGlanixState_IntroRoar(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_IntroRoar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_IntroRoar::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_RageChargeEnd.h"

#include "Glanix.h"
#include "Camera_Manager.h"

CGlanixState_RageChargeEnd::CGlanixState_RageChargeEnd(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageChargeEnd::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fSlidingSpeed = 10.f;

	return S_OK;
}

void CGlanixState_RageChargeEnd::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill07_Finish"));
}

void CGlanixState_RageChargeEnd::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimationFrame() <= 20)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), 10.f, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageChargeEnd::Exit_State()
{
}

CGlanixState_RageChargeEnd* CGlanixState_RageChargeEnd::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageChargeEnd* pInstance = new CGlanixState_RageChargeEnd(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageChargeEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageChargeEnd::Free()
{
	__super::Free();
}

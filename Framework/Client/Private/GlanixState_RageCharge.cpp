#include "stdafx.h"
#include "GlanixState_RageCharge.h"

#include "Glanix.h"

CGlanixState_RageCharge::CGlanixState_RageCharge(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageCharge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChargeSpeed = 20.f;
	m_fChargeTime = 2.f;

	return S_OK;
}

void CGlanixState_RageCharge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageSkillCharge"));
}

void CGlanixState_RageCharge::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimationFrame() <= 45)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());

	else if (m_pModelCom->Get_CurrAnimationFrame() >= 50)
	{
		/* 여기서 충돌 했을 때 Crash로 전환하면서 Crash 카운트 추가. */

		m_fTime += fTimeDelta;
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed, fTimeDelta);

		if (m_fTime >= m_fChargeTime)
		{
			m_fTime = m_fChargeTime - m_fTime;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGECHARGE_END);
		}
	}

	//if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	//{
	//	m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	//}
}

void CGlanixState_RageCharge::Exit_State()
{
}

CGlanixState_RageCharge* CGlanixState_RageCharge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageCharge* pInstance = new CGlanixState_RageCharge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageCharge::Free()
{
	__super::Free();
}

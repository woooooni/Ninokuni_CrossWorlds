#include "stdafx.h"
#include "GlanixState_Charge.h"

#include "Glanix.h"
#include "Camera_Manager.h"

CGlanixState_Charge::CGlanixState_Charge(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChargeSpeed = 20.f;

	return S_OK;
}

void CGlanixState_Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill07_RootMotion"));
}

void CGlanixState_Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 48 && m_pModelCom->Get_CurrAnimationFrame() <= 95)
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
	}

	if(m_pModelCom->Get_CurrAnimationFrame() >= 48 && m_pModelCom->Get_CurrAnimationFrame() <= 78)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed, fTimeDelta);
	else if(m_pModelCom->Get_CurrAnimationFrame() >= 78 && m_pModelCom->Get_CurrAnimationFrame() <= 95)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed / 2.f, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}

}

void CGlanixState_Charge::Exit_State()
{
}

CGlanixState_Charge* CGlanixState_Charge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Charge* pInstance = new CGlanixState_Charge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Charge::Free()
{
	__super::Free();
}


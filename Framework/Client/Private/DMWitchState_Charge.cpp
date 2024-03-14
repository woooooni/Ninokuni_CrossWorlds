#include "stdafx.h"
#include "DMWitchState_Charge.h"

#include "DMWitch.h"

CDMWitchState_Charge::CDMWitchState_Charge(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChargeSpeed = 20.f;

	return S_OK;
}

void CDMWitchState_Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill03"));
}

void CDMWitchState_Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 37 && m_pModelCom->Get_CurrAnimationFrame() <= 70)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}

}

void CDMWitchState_Charge::Exit_State()
{
}

CDMWitchState_Charge* CDMWitchState_Charge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Charge* pInstance = new CDMWitchState_Charge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Charge::Free()
{
	__super::Free();
}


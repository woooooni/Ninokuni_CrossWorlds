#include "stdafx.h"
#include "StelliaState_Charge.h"

#include "Stellia.h"

#include "Animation.h"

#include "Effect_Manager.h"

CStelliaState_Charge::CStelliaState_Charge(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChargeSpeed = 20.f;

	return S_OK;
}

void CStelliaState_Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06"));

	m_tChargeLerp.Start(m_fChargeSpeed, 0.f, 1.f, LERP_MODE::EASE_IN);

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Charge"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 35)
	{
		__super::Set_LockOffStellia();
	}


	m_pModelCom->Get_CurrAnimation()->Get_Duration();

	if (m_pModelCom->Get_CurrAnimationFrame() >= 65 && m_pModelCom->Get_CurrAnimationFrame() <= 90)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed, fTimeDelta);

	else if (m_pModelCom->Get_CurrAnimationFrame() >= 90 && m_pModelCom->Get_CurrAnimationFrame() <= 115)
	{
		m_tChargeLerp.Update(fTimeDelta);
		_float fCurValue = m_tChargeLerp.fCurValue;
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), fCurValue, fTimeDelta);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}

}

void CStelliaState_Charge::Exit_State()
{
	__super::Set_LockOnStellia();
}

CStelliaState_Charge* CStelliaState_Charge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Charge* pInstance = new CStelliaState_Charge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Charge::Free()
{
	__super::Free();
}


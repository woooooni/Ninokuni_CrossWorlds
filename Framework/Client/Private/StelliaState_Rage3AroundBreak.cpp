#include "stdafx.h"
#include "StelliaState_Rage3AroundBreak.h"

#include "Stellia.h"

#include "Animation.h"

CStelliaState_Rage3AroundBreak::CStelliaState_Rage3AroundBreak(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3AroundBreak::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3AroundBreak::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_End"));
	m_tChargeLerp.Start(m_fSprintSpeed * 2.f, 0.f, 2.f, LERP_MODE::EASE_IN);
}

void CStelliaState_Rage3AroundBreak::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_float fCurSpeed = m_tChargeLerp.Update(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() <= 22)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), fCurSpeed, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3TURN_PL);
	}

}

void CStelliaState_Rage3AroundBreak::Exit_State()
{
}

CStelliaState_Rage3AroundBreak* CStelliaState_Rage3AroundBreak::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3AroundBreak* pInstance = new CStelliaState_Rage3AroundBreak(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3AroundBreak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3AroundBreak::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "StelliaState_Rage1Loop_Idle.h"

#include "Stellia.h"
#include "Animation.h"

CStelliaState_Rage1Loop_Idle::CStelliaState_Rage1Loop_Idle(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Loop_Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1Loop_Idle::Enter_State(void* pArg)
{
	if (pArg != nullptr)
		m_eNextState = *(CStellia::STELLIA_STATE*)pArg;
	else
		m_eNextState = CStellia::STELLIA_RAGE1LOOP_TURN;

	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand02"));

	m_pStellia->Set_StelliaHit(false);
}

void CStelliaState_Rage1Loop_Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	__super::Rage1_Tick(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if(m_eNextState == CStellia::STELLIA_RAGE1LOOP_TURN)
			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_TURN);
		else if(m_eNextState == CStellia::STELLIA_RAGE1LOOP_EXPLOSION)
			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_EXPLOSION);
	}
}

void CStelliaState_Rage1Loop_Idle::Exit_State()
{
}

CStelliaState_Rage1Loop_Idle* CStelliaState_Rage1Loop_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Loop_Idle* pInstance = new CStelliaState_Rage1Loop_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Loop_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Loop_Idle::Free()
{
	__super::Free();
}


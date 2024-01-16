#include "stdafx.h"
#include "StelliaState_CounterEnd.h"

#include "Stellia.h"

CStelliaState_CounterEnd::CStelliaState_CounterEnd(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_CounterEnd::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_CounterEnd::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_CounterEnd"));
}

void CStelliaState_CounterEnd::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_CounterEnd::Exit_State()
{
}

CStelliaState_CounterEnd* CStelliaState_CounterEnd::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_CounterEnd* pInstance = new CStelliaState_CounterEnd(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_CounterEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_CounterEnd::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "StelliaState_CounterLoop.h"

#include "Stellia.h"

#include "GameInstance.h"

CStelliaState_CounterLoop::CStelliaState_CounterLoop(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_CounterLoop::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fCounterTime = 5.f;

	return S_OK;
}

void CStelliaState_CounterLoop::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_CounterLoop"));
	m_fTime = 0.f;
}

void CStelliaState_CounterLoop::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fCounterTime)
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COUNTEREND);
}

void CStelliaState_CounterLoop::Exit_State()
{
}

CStelliaState_CounterLoop* CStelliaState_CounterLoop::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_CounterLoop* pInstance = new CStelliaState_CounterLoop(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_CounterLoop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_CounterLoop::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "NpcState_Idle.h"

#include "GameNpc.h"

CNpcState_Idle::CNpcState_Idle(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcState_Idle::Exit_State()
{
}

CNpcState_Idle* CNpcState_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_Idle* pInstance = new CNpcState_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_Idle::Free()
{
	__super::Free();
}


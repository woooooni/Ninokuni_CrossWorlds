#include "stdafx.h"
#include "NpcDMWState_Idle.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcDMWState_Idle::CNpcDMWState_Idle(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcDMWState_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcDMWState_Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcDMWState_Idle::Exit_State()
{
}

CNpcDMWState_Idle* CNpcDMWState_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Idle* pInstance = new CNpcDMWState_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Idle::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "NpcState_Talk.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_Talk::CNpcState_Talk(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_Talk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_Talk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_Talk::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* 0. Talk / 1. Idle */
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if (__super::State_Wait(true, m_pModelCom->Get_Animation(m_AnimIndices[1])->Get_AnimationName(), 5.f, fTimeDelta))
		{
			m_pModelCom->Set_Animation(m_iCurrAnimIndex);
		}
	}
}

void CNpcState_Talk::Exit_State()
{
}

CNpcState_Talk* CNpcState_Talk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_Talk* pInstance = new CNpcState_Talk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_Talk::Free()
{
	__super::Free();
}


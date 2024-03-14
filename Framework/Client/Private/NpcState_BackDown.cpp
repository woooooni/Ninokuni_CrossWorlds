#include "stdafx.h"
#include "NpcState_BackDown.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_BackDown::CNpcState_BackDown(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_BackDown::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_BackDown::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_BackDown::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcState_BackDown::Exit_State()
{
}

CNpcState_BackDown* CNpcState_BackDown::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_BackDown* pInstance = new CNpcState_BackDown(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_BackDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_BackDown::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "NpcState_FrontDown.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_FrontDown::CNpcState_FrontDown(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_FrontDown::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_FrontDown::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_FrontDown::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcState_FrontDown::Exit_State()
{
}

CNpcState_FrontDown* CNpcState_FrontDown::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_FrontDown* pInstance = new CNpcState_FrontDown(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_FrontDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_FrontDown::Free()
{
	__super::Free();
}


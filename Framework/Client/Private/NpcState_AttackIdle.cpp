#include "stdafx.h"
#include "NpcState_AttackIdle.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_AttackIdle::CNpcState_AttackIdle(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_AttackIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_AttackIdle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_AttackIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcState_AttackIdle::Exit_State()
{
}

CNpcState_AttackIdle* CNpcState_AttackIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_AttackIdle* pInstance = new CNpcState_AttackIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_AttackIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_AttackIdle::Free()
{
	__super::Free();
}


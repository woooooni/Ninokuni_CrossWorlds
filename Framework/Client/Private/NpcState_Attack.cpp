#include "stdafx.h"
#include "NpcState_Attack.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_Attack::CNpcState_Attack(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_Attack::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_Attack::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_Attack::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcState_Attack::Exit_State()
{
}

CNpcState_Attack* CNpcState_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_Attack* pInstance = new CNpcState_Attack(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_Attack::Free()
{
	__super::Free();
}


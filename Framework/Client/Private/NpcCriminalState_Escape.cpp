#include "stdafx.h"
#include "NpcCriminalState_Escape.h"

#include "Criminal_Npc.h"

#include "Animation.h"

CNpcCriminalState_Escape::CNpcCriminalState_Escape(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcCriminalState_Escape::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcCriminalState_Escape::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcCriminalState_Escape::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.5f, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		m_pStateMachineCom->Change_State(CCriminal_Npc::NPC_CRIMINAL_STATE::NPC_CRIMINAL_ESCAPERUN);
}

void CNpcCriminalState_Escape::Exit_State()
{
}

CNpcCriminalState_Escape* CNpcCriminalState_Escape::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcCriminalState_Escape* pInstance = new CNpcCriminalState_Escape(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcCriminalState_Escape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcCriminalState_Escape::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "NpcDMWState_InvasionDisappear.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

CNpcDMWState_InvasionDisappear::CNpcDMWState_InvasionDisappear(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_InvasionDisappear::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fWalkTime = 5.f;

	return S_OK;
}

void CNpcDMWState_InvasionDisappear::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_fTime = 0.f;
}

void CNpcDMWState_InvasionDisappear::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	m_fTime += fTimeDelta;

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pWitch->Get_Stat()->fSpeed, fTimeDelta);

	if (m_fTime >= m_fWalkTime)
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_INVASION_IDLE);
	}
}

void CNpcDMWState_InvasionDisappear::Exit_State()
{
}

CNpcDMWState_InvasionDisappear* CNpcDMWState_InvasionDisappear::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_InvasionDisappear* pInstance = new CNpcDMWState_InvasionDisappear(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_InvasionDisappear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_InvasionDisappear::Free()
{
	__super::Free();
}


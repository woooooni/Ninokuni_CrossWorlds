#include "stdafx.h"
#include "NpcDMWState_Following.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

CNpcDMWState_Following::CNpcDMWState_Following(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Following::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcDMWState_Following::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);
}

void CNpcDMWState_Following::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CNpcDMWState_Following::Exit_State()
{
}

CNpcDMWState_Following* CNpcDMWState_Following::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Following* pInstance = new CNpcDMWState_Following(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Following");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Following::Free()
{
	__super::Free();
}


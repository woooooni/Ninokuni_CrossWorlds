#include "stdafx.h"
#include "NpcDMWState_Following_Rage03.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Following_Rage03::CNpcDMWState_Following_Rage03(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Following_Rage03::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fCoolTime = 5.f;

	return S_OK;
}

void CNpcDMWState_Following_Rage03::Enter_State(void* pArg)
{
	// m_pStellia = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_MONSTER, TEXT("Stellia"));

	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(false);
	m_pWitch->Set_IsFollowingPlayer(true);
}

void CNpcDMWState_Following_Rage03::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	
	if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_COUNTERSTART)
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
		m_pWitch->Set_IsFollowing(true);
		m_pWitch->Set_IsFollowingPlayer(false);

		return;
	}

	if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE3CHARGE_READY &&
		m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE3CHARGE &&
		m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE3CHARGE_BREAK &&
		m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE3DAMAGE &&
		m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE3AROUND_BREAK)
	{
		if (m_pWitch->Get_IsBattle())
		{
			m_fAccCoolTime += fTimeDelta;

			if (m_fAccCoolTime > m_fCoolTime)
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_RAGE3_LASER);
				m_fAccCoolTime = m_fCoolTime - m_fAccCoolTime;
			}
		}
	}
}

void CNpcDMWState_Following_Rage03::Exit_State()
{
}

CNpcDMWState_Following_Rage03* CNpcDMWState_Following_Rage03::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Following_Rage03* pInstance = new CNpcDMWState_Following_Rage03(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Following_Rage03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Following_Rage03::Free()
{
	__super::Free();
}

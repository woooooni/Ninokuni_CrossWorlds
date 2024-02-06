#include "stdafx.h"
#include "NpcDMWState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "DreamMazeWitch_Npc.h"

#include "Game_Manager.h"
#include "Player.h"

_uint CNpcDMWState_Base::m_iAtkIndex = 0;

CNpcDMWState_Base::CNpcDMWState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CNpcDMWState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pWitch = dynamic_cast<CDreamMazeWitch_Npc*>(m_pOwner);
	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fFollowingSpeed = 12.f;

	/* 공격 패턴만 따로 모아놓기. (후에 순차적 혹은 랜덤으로 전환하기 위해) */
	m_vecAtkState.push_back(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_ATTACK);
	m_vecAtkState.push_back(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_ATTACK);
	// m_vecAtkState.push_back(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_LASER);


	return S_OK;
}

void CNpcDMWState_Base::Enter_State(void* pArg)
{

}

void CNpcDMWState_Base::Tick_State(_float fTimeDelta)
{
	if (m_pStellia == nullptr)
	{
		m_pStellia = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_MONSTER, TEXT("Stellia"));
	}

	if (m_pStellia != nullptr)
	{
		if (m_pWitch->Get_IsBattle())
		{
			if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE2START)
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING_RAGE02);
			}
			else if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE3START_FADEIN)
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING_RAGE03);
			}
		}
	}
}

void CNpcDMWState_Base::Exit_State()
{
}

void CNpcDMWState_Base::Free()
{
	__super::Free();
}

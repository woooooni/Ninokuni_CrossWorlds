#include "stdafx.h"
#include "NpcDMWState_Attack.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Attack::CNpcDMWState_Attack(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Attack::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_iCount = 5;

	return S_OK;
}

void CNpcDMWState_Attack::Enter_State(void* pArg)
{
	m_pStellia = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_MONSTER, TEXT("Stellia"));

	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);

	m_bIsAttack = false;
}

void CNpcDMWState_Attack::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (!m_bIsAttack)
	{
		for (_uint i = 0; i < m_iCount; ++i)
		{
			Vec4 vBlackHolePos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();
			vBlackHolePos.x += GI->RandomFloat(-7.f, 7.f);
			vBlackHolePos.y += 2.f;
			vBlackHolePos.z += GI->RandomFloat(-7.f, 7.f);

			CGameObject* pBlockHole = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_BlackHole", m_pStellia, &pBlockHole)))
			{
				MSG_BOX("Add BlackHole Failed.");
				return;
			}

			CTransform* pBlackHoleTransform = pBlockHole->Get_Component_Transform();

			pBlackHoleTransform->Set_State(CTransform::STATE_POSITION, vBlackHolePos);
		
		}

		m_bIsAttack = true;
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
	}
}

void CNpcDMWState_Attack::Exit_State()
{
}

CNpcDMWState_Attack* CNpcDMWState_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Attack* pInstance = new CNpcDMWState_Attack(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Attack::Free()
{
	__super::Free();
}


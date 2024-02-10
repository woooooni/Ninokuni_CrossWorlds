#include "stdafx.h"
#include "NpcDMWState_Rage01QuadBlackHole.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Rage01QuadBlackHole::CNpcDMWState_Rage01QuadBlackHole(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Rage01QuadBlackHole::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_iCount = 4;
	m_fDist = 7.f;

	m_fSpawnTime = 0.1f;

	return S_OK;
}

void CNpcDMWState_Rage01QuadBlackHole::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	// m_pWitch->Set_IsFollowing(true);

	m_bIsAttack = false;
	m_fAccTime = 0.f;
	m_iCurCount = 0;
}

void CNpcDMWState_Rage01QuadBlackHole::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fAccTime += fTimeDelta;

	if (!m_bIsAttack && m_fAccTime > m_fSpawnTime)
	{
		m_fAccTime = m_fSpawnTime - m_fAccTime;

		if (m_iCurCount >= m_iCount)
		{
			m_bIsAttack = true;
		}
		else
		{
			Vec4 vStelliaPos = m_pStellia->Get_Component_Transform()->Get_Position();
			Vec4 vStelliaLook = m_pStellia->Get_Component_Transform()->Get_Look();
			Vec4 vStelliaRight = m_pStellia->Get_Component_Transform()->Get_Right();

			Vec4 vBlackHolePos = {};
			switch (m_iCurCount)
			{
			case 0:
				vBlackHolePos = vStelliaPos + (vStelliaLook * m_fDist);
				break;
			case 1:
				vBlackHolePos = vStelliaPos + (vStelliaRight * m_fDist);
				break;
			case 2:
				vBlackHolePos = vStelliaPos + (-vStelliaLook * m_fDist);
				break;
			case 3:
				vBlackHolePos = vStelliaPos + (-vStelliaRight * m_fDist);
				break;
			}

			vBlackHolePos.y += 2.f;

			CGameObject* pBlackHole = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_Rage01QuadBlackHole", m_pStellia, &pBlackHole)))
			{
				MSG_BOX("Add QuadBlackHole Failed.");
				return;
			}

			CTransform* pBlackHoleTransform = pBlackHole->Get_Component_Transform();

			pBlackHoleTransform->Set_State(CTransform::STATE_POSITION, vBlackHolePos);

			m_iCurCount += 1;
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
	}
}

void CNpcDMWState_Rage01QuadBlackHole::Exit_State()
{
}

CNpcDMWState_Rage01QuadBlackHole* CNpcDMWState_Rage01QuadBlackHole::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Rage01QuadBlackHole* pInstance = new CNpcDMWState_Rage01QuadBlackHole(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Rage01QuadBlackHole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Rage01QuadBlackHole::Free()
{
	__super::Free();
}


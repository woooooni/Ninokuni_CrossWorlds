#include "stdafx.h"
#include "NpcDMWState_Attack.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "Vfx.h"
#include "Vfx_Witch_Attack.h"

CNpcDMWState_Attack::CNpcDMWState_Attack(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Attack::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_iCount = 5;

	m_fSpawnTime = 0.15f;

	return S_OK;
}

void CNpcDMWState_Attack::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	CSound_Manager::GetInstance()->Play_Sound(TEXT("DreamersMazeWitch_V_Atk_2.ogg"), CHANNELID::SOUND_VOICE_NPC, 1.f, true);

	m_pWitch->Set_IsFollowing(true);

	m_bIsAttack = false;
	m_fAccTime = 0.f;
	m_iCurCount = 0;

	// Effect Create
	//CVfx* pVfxEffect = nullptr;
	//GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Attack"), m_pTransformCom->Get_WorldMatrix(), m_pWitch, //&pVfxEffect);
	//if (nullptr != pVfxEffect)
	//	pVfxEffect->Set_OwnerStateIndex((_int)CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_ATTACK);

}

void CNpcDMWState_Attack::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (CGame_Manager::GetInstance()->Get_Player()->Get_Character() != nullptr)
	{
		/* 우선 순위 스킬들 */
		if (m_pStellia != nullptr)
		{
			// 스텔리아가 점프 스탬프라면
			if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_JUMPSTAMP &&
				m_pStellia->Get_Component_Model()->Get_CurrAnimationFrame() < 10)
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_VULCAN_READY);
			}
			// 스텔리아가 차지 레디 상태라면
			if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_CHARGE &&
				m_pStellia->Get_Component_Model()->Get_CurrAnimationFrame() < 10)
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_CHARGE_READY);
			}
		}

		/* 우선 순위 스킬이 아니라면 */
		Vec4 vPlayerPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();
		m_pTransformCom->LookAt_ForLandObject(vPlayerPos);

		if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack02"))
		{
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
					Vec4 vBlackHolePos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();

					if (m_iCurCount == 0)
					{
						vBlackHolePos.x += 2.f;
						vBlackHolePos.y += 2.f;
						vBlackHolePos.z += 2.f;
					}

					else
					{
						vBlackHolePos.x += GI->RandomFloat(-10.f, 10.f);
						vBlackHolePos.y += 2.f;
						vBlackHolePos.z += GI->RandomFloat(-10.f, 10.f);
					}

					CGameObject* pBlockHole = nullptr;

					if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_BlackHole", m_pStellia, &pBlockHole)))
					{
						MSG_BOX("Add BlackHole Failed.");
						return;
					}

					CTransform* pBlackHoleTransform = pBlockHole->Get_Component_Transform();

					pBlackHoleTransform->Set_State(CTransform::STATE_POSITION, vBlackHolePos);

					m_iCurCount += 1;
				}
			}
		}
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


#include "stdafx.h"
#include "NpcDMWState_Rage03Laser.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Rage03Laser::CNpcDMWState_Rage03Laser(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Rage03Laser::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_iCount = 5;

	m_fSpawnTime = 0.3f;

	return S_OK;
}

void CNpcDMWState_Rage03Laser::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	CSound_Manager::GetInstance()->Play_Sound(TEXT("DreamersMazeWitch_V_Atk_2.ogg"), CHANNELID::SOUND_VOICE_NPC, 1.f, true);

	// m_pWitch->Set_IsFollowing(true);

	m_bIsAttack = false;
	m_fAccTime = 0.f;
	m_iCurCount = 0;
}

void CNpcDMWState_Rage03Laser::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

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
				Vec4 vLaserPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();
				vLaserPos.x += GI->RandomFloat(-15.f, 15.f);
				vLaserPos.y += 0.f;
				vLaserPos.z += GI->RandomFloat(-15.f, 15.f);

				CGameObject* pLaser = nullptr;

				if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_Laser", m_pStellia, &pLaser)))
				{
					MSG_BOX("Add Laser Failed.");
					return;
				}

				CTransform* pLaserTransform = pLaser->Get_Component_Transform();

				pLaserTransform->Set_State(CTransform::STATE_POSITION, vLaserPos);
				pLaserTransform->FixRotation(0.f, GI->RandomFloat(10.f, 170.f), 0.f);

				m_iCurCount += 1;
			}
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING_RAGE03);
	}
}

void CNpcDMWState_Rage03Laser::Exit_State()
{
}

CNpcDMWState_Rage03Laser* CNpcDMWState_Rage03Laser::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Rage03Laser* pInstance = new CNpcDMWState_Rage03Laser(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Rage03Laser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Rage03Laser::Free()
{
	__super::Free();
}


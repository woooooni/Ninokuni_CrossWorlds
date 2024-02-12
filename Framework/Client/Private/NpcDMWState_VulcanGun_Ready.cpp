#include "stdafx.h"
#include "NpcDMWState_VulcanGun_Ready.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_VulcanGun_Ready::CNpcDMWState_VulcanGun_Ready(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_VulcanGun_Ready::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_fDist = 15.f;
	m_fMoveTime = 1.f;
	m_fDownTime = 1.f;

	return S_OK;
}

void CNpcDMWState_VulcanGun_Ready::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	CSound_Manager::GetInstance()->Play_Sound(TEXT("DreamersMazeWitch_V_Skill_6_1.ogg"), CHANNELID::SOUND_VOICE_NPC, 1.f, true);

	m_bIsMove = false;
	m_bIsDown = false;
}

void CNpcDMWState_VulcanGun_Ready::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pStellia != nullptr)
	{
		// 스텔리아 멈추기
		if (m_pStellia->Get_Component_Model()->Get_CurrAnimationFrame() > 45)
		{
			m_pStellia->Get_Component_Model()->Set_Stop_Animation(true);

			//m_pStellia->Get_Component_Rigidbody()->Set_Ground(true);
			//m_pStellia->Get_Component_Rigidbody()->Set_Use_Gravity(false);
			//m_pStellia->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_Active(false);
		}

		// 한 번만 세팅
		if (!m_bIsMove)
		{
			Vec4 vStelliaPos = m_pStellia->Get_Component_Transform()->Get_Position();
			Vec3 vStelliaLook = m_pStellia->Get_Component_Transform()->Get_Look();
			vStelliaLook.Normalize();

			m_vDestPos = vStelliaPos + (vStelliaLook * m_fDist);
			m_vDestPos.y += 5.f;
			m_tMoveLerp.Start(m_pTransformCom->Get_Position(), m_vDestPos, m_fMoveTime, LERP_MODE::SMOOTH_STEP);

			m_bIsMove = true;
		}

		if (CGame_Manager::GetInstance()->Get_Player()->Get_Character() != nullptr)
		{
			m_pTransformCom->LookAt_ForLandObject(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position());
			Vec4 vCurPos = m_pTransformCom->Get_Position();
			Vec4 vPlayerPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();

			// 플레이어를 바라보면서 이동
			if (m_tMoveLerp.bActive)
			{
				vCurPos = m_tMoveLerp.Update_Lerp(fTimeDelta);
				m_pTransformCom->Set_Position(vCurPos);
			}
			// 이동이 끝났으면
			else
			{
				// 다운을 위한 세팅 시작.
				if (!m_bIsDown)
				{
					m_tDownLerp.Start(vCurPos.y, 0.5f, m_fDownTime, LERP_MODE::SMOOTH_STEP);
					m_bIsDown = true;
				}
				// 세팅이 되었다면 다운.
				else
				{
					if (m_tDownLerp.bActive)
					{
						vCurPos.y = m_tDownLerp.Update(fTimeDelta);
						m_pTransformCom->Set_Position(vCurPos);
					}
					// 모든 이동, 다운이 끝났으면 공격 시작
					else
					{
						m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_VULCAN_ATTACK);
					}
				}
			}
		}

	}
}

void CNpcDMWState_VulcanGun_Ready::Exit_State()
{
}

CNpcDMWState_VulcanGun_Ready* CNpcDMWState_VulcanGun_Ready::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_VulcanGun_Ready* pInstance = new CNpcDMWState_VulcanGun_Ready(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_VulcanGun_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_VulcanGun_Ready::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "NpcDMWState_Charge_End.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Charge_End::CNpcDMWState_Charge_End(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Charge_End::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_fMoveTime = 1.5f;
	m_fUpTime = 1.f;
	m_fDestY = 7.f;

	return S_OK;
}

void CNpcDMWState_Charge_End::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);

	m_bIsMove = false;
	m_bIsUp = false;
}

void CNpcDMWState_Charge_End::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pStellia != nullptr)
	{
		// 한 번만 세팅
		if (!m_bIsUp)
		{
			Vec4 vCurPos = m_pTransformCom->Get_Position();

			m_tUpLerp.Start(vCurPos.y, m_fDestY, m_fUpTime, LERP_MODE::SMOOTH_STEP);
			m_bIsUp = true;
		}

		// 올라가기
		if (m_bIsUp)
		{
			if (m_tUpLerp.bActive)
			{
				Vec4 vCurPos = m_pTransformCom->Get_Position();

				vCurPos.y = m_tUpLerp.Update(fTimeDelta);
				m_pTransformCom->Set_Position(vCurPos);
			}
			else
			{
				// 다 올라왔으면 무브 세팅
				if (!m_bIsMove)
				{
					Vec4 vStelliaPos = m_pStellia->Get_Component_Transform()->Get_Position();
					vStelliaPos.y = m_fDestY;
					m_tMoveLerp.Start(m_pTransformCom->Get_Position(), vStelliaPos, m_fMoveTime, LERP_MODE::SMOOTH_STEP);
					m_bIsMove = true;
				}
			}
		}

		// 이동하기
		if (m_bIsMove)
		{
			if (m_tMoveLerp.bActive)
			{
				Vec4 vCurPos = m_pTransformCom->Get_Position();

				vCurPos = m_tMoveLerp.Update_Lerp(fTimeDelta);
				m_pTransformCom->Set_Position(vCurPos);
			}
			// 다 도착했으면 상태 전환.
			else
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
			}
		}


		if (CGame_Manager::GetInstance()->Get_Player()->Get_Character() != nullptr)
		{
			m_pTransformCom->LookAt_ForLandObject(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position());
		}

	}
}

void CNpcDMWState_Charge_End::Exit_State()
{
}

CNpcDMWState_Charge_End* CNpcDMWState_Charge_End::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Charge_End* pInstance = new CNpcDMWState_Charge_End(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Charge_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Charge_End::Free()
{
	__super::Free();
}


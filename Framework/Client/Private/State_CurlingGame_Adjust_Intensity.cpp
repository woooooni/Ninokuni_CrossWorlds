#include "stdafx.h"
#include "State_CurlingGame_Adjust_Intensity.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"


CState_CurlingGame_Adjust_Intensity::CState_CurlingGame_Adjust_Intensity(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Adjust_Intensity::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Adjust_Intensity::Enter_State(void* pArg)
{
	if (nullptr == m_pManager->m_pCurStone)
		return;

	m_tGuageDesc.Start();
}

void CState_CurlingGame_Adjust_Intensity::Tick_State(const _float& fTimeDelta)
{
	if (!m_tGuageDesc.bActive)
	{
		
	}
	else
	{
		m_tGuageDesc.Tick(fTimeDelta);

		if (m_pManager->m_bPlayerTurn)
		{
			if (KEY_AWAY(KEY::SPACE))
			{
				m_pManager->m_pCurStone->Launch(m_pManager->m_vCurStoneLook.ZeroY().Normalized(), m_tGuageDesc.fMaxPower * m_tGuageDesc.tLerpValue.fCurValue);

				m_pManager->m_pStonesLaunched.push_back(m_pManager->m_pCurStone);

				m_tGuageDesc.Stop();

				m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iNumStone--;

				if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::LAUNCH)))
					return;
			}

		}
		else
		{
			if (m_pManager->m_tCurAiPath.fPower <= m_tGuageDesc.tLerpValue.fCurValue)
			{
				m_pManager->m_pCurStone->Launch(m_pManager->m_vCurStoneLook.ZeroY().Normalized(), m_tGuageDesc.fMaxPower * m_tGuageDesc.tLerpValue.fCurValue);

				m_pManager->m_pStonesLaunched.push_back(m_pManager->m_pCurStone);

				m_tGuageDesc.Stop();

				m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iNumStone--;

				if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::LAUNCH)))
					return;
			}
		}
	}

	/* Send To Ui */
	{
		CUI_Minigame_Curling_Base* pUi = CUIMinigame_Manager::GetInstance()->Get_MiniGame_Curling_Ui(MG_CURLING_UI_TYPE::GUAGE);
		if(nullptr != pUi)
			pUi->Send_Message_Float(m_tGuageDesc.tLerpValue.fCurValue);
	}
}

void CState_CurlingGame_Adjust_Intensity::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Adjust_Intensity::Exit_State()
{
}

HRESULT CState_CurlingGame_Adjust_Intensity::Render()
{
	return S_OK;
}

CState_CurlingGame_Adjust_Intensity* CState_CurlingGame_Adjust_Intensity::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Adjust_Intensity* pInstance = new CState_CurlingGame_Adjust_Intensity(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Adjust_Intensity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Adjust_Intensity::Free()
{
	__super::Free();
}


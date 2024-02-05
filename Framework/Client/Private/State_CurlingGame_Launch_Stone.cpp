#include "stdafx.h"
#include "State_CurlingGame_Launch_Stone.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"
#include "State_CurlingGame_Move_Character.h"

#include "Player.h"
#include "Character.h"

#include "Animation.h"

CState_CurlingGame_Launch_Stone::CState_CurlingGame_Launch_Stone(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Launch_Stone::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Launch_Stone::Enter_State(void* pArg)
{
	/* 댐핑 시작 */
	{
		CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr == pCurlingCam)
			return;

		pCurlingCam->Start_StoneAction();
	}

	/* 스톤 갯수 감소 */
	if (m_pManager->m_bPlayerTurn)
		m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iNumStone--;
	else
		m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iNumStone--;

	/* Send To Ui */
	Send_To_Ui();
}

void CState_CurlingGame_Launch_Stone::Tick_State(const _float& fTimeDelta)
{
	/* 모든 스톤이 움직임을 멈췄는지 체크한다. */
	if (!Check_AllStoneStop())
		return;

	/* 데이터를 취합하여 게임 종료 여부를 판별한다. */
	if (!m_bCheckFinishGame)
	{
		m_bCheckFinishGame = true;

		/* 점수를 갱신한다. */
		Calculate_Score();

		/* 아웃된 스톤을 정리한다. */
		for (auto& pStone : m_pManager->m_pStonesLaunched)
		{
			if (pStone->Is_Outted() && pStone->Is_Active())
				pStone->Set_Active(false);
		}

		/* 게이지 UI를 초기화 한다. */
		{
			CUI_Minigame_Curling_Base* pUi = CUIMinigame_Manager::GetInstance()->Get_MiniGame_Curling_Ui((_uint)MG_CL_UI_TYPE::GUAGE);
			if (nullptr != pUi)
				pUi->Send_Message();
		}

		/* 남은 스톤개수를 바탕으로 게임 종료 여부를 결정한다. */
		if (Check_FinishGame())
		{
			const _uint iPlayerScore = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iScore;
			const _uint iNpcScore = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iScore;

			if (iPlayerScore == iNpcScore) /* 비기면 스톤 추가*/
			{
				m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iNumStone = 2;
				m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iNumStone = 2;
				Send_To_Ui();
			}
			else
			{
				if (iPlayerScore > iNpcScore)		/* 플레이어 승리 */
					m_pManager->m_bPlayerWin = true;
				else if (iPlayerScore < iNpcScore)	/* 플레이어 패배 */
					m_pManager->m_bPlayerWin = false;

				/* 게임이 종료될 것이라면 바로 엔딩스테이트 전환 */
				if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::ENDING)))
					return;

				return;
			}
		}
	}

	if (!m_bResetTurn)
	{
		m_bResetTurn = true;

		/* 턴 갱신 */
		if (FAILED(m_pManager->Change_Turn()))
			return;

		/* 카메라 리셋 블렌딩 시작 */
		CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pCurlingCam)
		{
			/* 순서 변경 유의 */
			pCurlingCam->Finish_StoneAction(m_fTargetChangeLerpDuration);
			pCurlingCam->Change_Target(m_pManager->m_pCurParticipant, m_fTargetChangeLerpDuration);
		}
	}

	/* Npc의 경우 스톤 트랜스폼 설정 */
	if (!m_pManager->m_bPlayerTurn && !m_bSetNpcStoneTransform)
	{
		m_fAcc += fTimeDelta;
		if (m_fNpcStoneLimit <= m_fAcc)
		{
			m_bSetNpcStoneTransform = true;

			m_pManager->m_pCurParticipant->Get_Component_Model()->Set_KeyFrame_By_Progress(0.8f);

			CManager_State* pState = m_pManager->m_pManagerStateMachineCom->Get_State(CCurlingGame_Manager::CURLINGGAME_STATE::MOVE);
			if (nullptr != pState)
			{
				CState_CurlingGame_Move_Character* pMoveState = dynamic_cast<CState_CurlingGame_Move_Character*>(pState);
				if (nullptr != pMoveState)
					pMoveState->Set_NpcStoneTransform();
			}
		}
	}

	/* 카메라 타겟 변경 블렌딩 완료 여부 체크 */
	CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	if (!pCurlingCam->Is_ChagingTarget())
	{
		/* 다음 턴으로 스테이트 변경 */
		if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::MOVE)))
			return;
	}
}

void CState_CurlingGame_Launch_Stone::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Launch_Stone::Exit_State()
{
	m_bCheckFinishGame	= false;
	m_bChangeTarget		= false;
	m_bResetTurn		= false;

	m_bSetNpcStoneTransform	= false;
	m_fAcc = 0.f;
}

HRESULT CState_CurlingGame_Launch_Stone::Render()
{
	return S_OK;
}

const _bool& CState_CurlingGame_Launch_Stone::Check_AllStoneStop()
{
	for (auto& pStone : m_pManager->m_pStonesLaunched)
	{
		if (nullptr == pStone || pStone->Is_Outted())
			continue;

		if (pStone->Is_Moving())
			return false;
	}

	return true;
}

const _bool& CState_CurlingGame_Launch_Stone::Check_FinishGame()
{
	if (0 == m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iNumStone
		&& 0 == m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iNumStone)
		return true;
	
	return false;
}

void CState_CurlingGame_Launch_Stone::Calculate_Score()
{
	const _uint iPrevScores[CCurlingGame_Manager::PARTICIPANT_TYPEEND] =
	{
		m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iScore,
		m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iScore
	};

	_uint iCurScores[CCurlingGame_Manager::PARTICIPANT_TYPEEND] = { 0, 0 };

	for (auto& pStone : m_pManager->m_pStonesLaunched)
	{
		/* Exception */
		if (nullptr == pStone || pStone->Is_Outted())
			continue;

		/* Calculate Point */
		_int iPoint = 0;
		{
			const _float fDistance = Vec3::Distance(pStone->Get_Transform()->Get_Position(), m_pManager->m_tStandardDesc.vGoalPosition);

			if (fDistance <= m_pManager->m_tStandardDesc.fRingScalesForDetection[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::FIRST])
			{
				iPoint = m_pManager->m_tStandardDesc.iPoints[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::FIRST];
			}
			else if (m_pManager->m_tStandardDesc.fRingScalesForDetection[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::FIRST] <= fDistance
				&& fDistance < m_pManager->m_tStandardDesc.fRingScalesForDetection[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::SECOND])
			{
				iPoint = m_pManager->m_tStandardDesc.iPoints[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::SECOND];
			}
			else if (m_pManager->m_tStandardDesc.fRingScalesForDetection[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::SECOND] <= fDistance
				&& fDistance < m_pManager->m_tStandardDesc.fRingScalesForDetection[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::THIRD])
			{
				iPoint = m_pManager->m_tStandardDesc.iPoints[CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::THIRD];
			}
		}

		/* Accumulate */
		switch (pStone->Get_StoneType())
		{
		case CCurlingGame_Stone::STONE_TYPE::BARREL:
		{
			iCurScores[CCurlingGame_Manager::PARTICIPANT_PLAYER] += iPoint;
			pStone->Set_Point(iPoint);
		}
		break;
		case CCurlingGame_Stone::STONE_TYPE::POT:
		{
			iCurScores[CCurlingGame_Manager::PARTICIPANT_NPC] += iPoint;
			pStone->Set_Point(iPoint);
		}
		break;
		default:
			break;
		}
	}

	m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iScore = iCurScores[CCurlingGame_Manager::PARTICIPANT_PLAYER];
	m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iScore = iCurScores[CCurlingGame_Manager::PARTICIPANT_NPC];

	/* Send To Ui - Score */
	{
		/* Player */
		{
			CUI_Minigame_Curling_Base* pUi = CUIMinigame_Manager::GetInstance()->Get_MiniGame_Curling_Ui((_uint)MG_CL_UI_TYPE::SCORE_PLAYER);
			if (nullptr != pUi)
				pUi->Send_Message_Int(m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iScore);
		}
		/* Npc */
		{
			CUI_Minigame_Curling_Base* pUi = CUIMinigame_Manager::GetInstance()->Get_MiniGame_Curling_Ui((_uint)MG_CL_UI_TYPE::SCORE_NPC);
			if (nullptr != pUi)
				pUi->Send_Message_Int(m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iScore);

		}
	}

}

void CState_CurlingGame_Launch_Stone::Send_To_Ui()
{
	/* Player */
	{
		CUI_Minigame_Curling_Base* pUi = CUIMinigame_Manager::GetInstance()->Get_MiniGame_Curling_Ui((_uint)MG_CL_UI_TYPE::STONES_PLAYER);
		if (nullptr != pUi)
			pUi->Send_Message_Int(m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_TYPE::PARTICIPANT_PLAYER].iNumStone);
	}

	/* Npc */
	{
		CUI_Minigame_Curling_Base* pUi = CUIMinigame_Manager::GetInstance()->Get_MiniGame_Curling_Ui((_uint)MG_CL_UI_TYPE::STONES_NPC);
		if (nullptr != pUi)
			pUi->Send_Message_Int(m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_TYPE::PARTICIPANT_NPC].iNumStone);
	}

}

void CState_CurlingGame_Launch_Stone::Change_Turn()
{
}

CState_CurlingGame_Launch_Stone* CState_CurlingGame_Launch_Stone::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Launch_Stone* pInstance = new CState_CurlingGame_Launch_Stone(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Launch_Stone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Launch_Stone::Free()
{
	__super::Free();
}


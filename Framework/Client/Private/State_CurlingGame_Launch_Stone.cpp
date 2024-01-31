#include "stdafx.h"
#include "State_CurlingGame_Launch_Stone.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

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
}

void CState_CurlingGame_Launch_Stone::Tick_State(const _float& fTimeDelta)
{
	/* 발사된 모든 스톤이 정지상태라면 */
	if (!m_bResetTurn && Check_AllStoneStop())
	{
		m_bResetTurn = true;

		/* 점수를 갱신한다. */
		Calculate_Score(); 

		/* 턴 갱신 */
		if (FAILED(m_pManager->Change_Turn()))
			return;

		/* 카메라 리셋 블렌딩 시작 */
		CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pCurlingCam)
		{
			const _float fLerpTime = 0.75f;

			/* 순서 변경 유의 */
			pCurlingCam->Finish_StoneAction(fLerpTime);
			pCurlingCam->Change_Target(m_pManager->m_pCurParticipant, fLerpTime);
		}
	}

	if (m_bResetTurn)
	{
		CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pCurlingCam)
		{
			if (!pCurlingCam->Is_ChagingTarget())
			{
				if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::MOVE)))
					return;
			}
		}
	}
}

void CState_CurlingGame_Launch_Stone::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Launch_Stone::Exit_State()
{
	m_bResetTurn = false;
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
		}
		break;
		case CCurlingGame_Stone::STONE_TYPE::POT:
		{
			iCurScores[CCurlingGame_Manager::PARTICIPANT_NPC] += iPoint;
		}
		break;
		default:
			break;
		}
	}

	m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iScore = iCurScores[CCurlingGame_Manager::PARTICIPANT_PLAYER];
	m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iScore = iCurScores[CCurlingGame_Manager::PARTICIPANT_NPC];
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


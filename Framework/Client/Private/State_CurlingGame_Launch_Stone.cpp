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

		/* 턴 변경 */
		m_pManager->m_bPlayerTurn = !m_pManager->m_bPlayerTurn;

		/* 다음 턴 캐릭터 트랜스폼, 애니메이션 설정 */
		CGameObject* pChangeTarget = nullptr;
		{
			if (m_pManager->m_bPlayerTurn)
				pChangeTarget = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner;
			else
				pChangeTarget = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].pOwner;

			const Vec4 vStartPos = Vec4(m_pManager->m_tStandardDesc.vStartLinePosition + (m_pManager->m_tStandardDesc.vStartLook * -5.f)).OneW();

			pChangeTarget->Get_Component_Transform()->Set_Position(vStartPos);
			pChangeTarget->Get_Component_Transform()->LookAt_ForLandObject(m_pManager->m_tStandardDesc.vGoalPosition);

			pChangeTarget->Get_Component_Model()->Set_CanChangeAnimation(true);
		}

		/* 카메라 리셋 블렌딩 시작 */
		CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pCurlingCam)
		{
			const _float fLerpTime = 0.75f;

			/* 순서 변경 유의 */
			pCurlingCam->Finish_StoneAction(fLerpTime);
			pCurlingCam->Change_Target(pChangeTarget, fLerpTime);
		}

		/* 이전 턴 캐릭터 트랜스폼, 애니메이션 설정 */
		{
			const Vec4 vWaitPos = Vec4(m_pManager->m_tStandardDesc.vStartLinePosition + (m_pManager->m_tStandardDesc.vStartLook * -6.f)).OneW();

			CGameObject* pWaitCharacter = nullptr;
			_float fDeltaX = 0.f;

			if (m_pManager->m_bPlayerTurn)
			{
				pWaitCharacter = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].pOwner;
				pWaitCharacter->Get_Component_Model()->Set_Animation(L"SKM_Destroyer_Merge.ao|Destroyer_ChairSitLoop");
				CAnimation* pAnim = pWaitCharacter->Get_Component_Model()->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_ChairSitLoop");
				if (nullptr != pAnim)
				{
					pAnim->Set_Loop(true);
				}
				fDeltaX = -2.f;
			}
			else
			{
				pWaitCharacter = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner;
				pWaitCharacter->Get_Component_Model()->Set_Animation(L"SKM_Swordsman_Merge.ao|Swordsman_ChairSitLoop");
				CAnimation* pAnim = pWaitCharacter->Get_Component_Model()->Get_Animation("SKM_Swordsman_Merge.ao|Swordsman_ChairSitLoop");
				if (nullptr != pAnim)
				{
					pAnim->Set_Loop(true);
				}
				fDeltaX = 2.f;
			}
			pWaitCharacter->Get_Component_Model()->Set_CanChangeAnimation(false);
			pWaitCharacter->Get_Component_Transform()->Set_Position(vWaitPos);
			pWaitCharacter->Get_Component_Transform()->LookAt_ForLandObject(m_pManager->m_tStandardDesc.vGoalPosition);

			const Vec4 vOffset = pWaitCharacter->Get_Component_Transform()->Get_RelativeOffset(Vec4{ fDeltaX, 0.f, 0.f, 1.f }).ZeroW();
			pWaitCharacter->Get_Component_Transform()->Set_Position(vWaitPos + vOffset);
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


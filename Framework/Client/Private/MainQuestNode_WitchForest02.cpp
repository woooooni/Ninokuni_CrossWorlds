#include "stdafx.h"
#include "MainQuestNode_WitchForest02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"

#include "Player.h"
#include "Character.h"
#include "SkyDome.h"
#include "Quest_Manager.h"

CMainQuestNode_WitchForest02::CMainQuestNode_WitchForest02()
{
}

HRESULT CMainQuestNode_WitchForest02::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CMainQuestNode_WitchForest02::Start()
{
}

CBTNode::NODE_STATE CMainQuestNode_WitchForest02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_WITCHFOREST)
	{
		if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CurrentState() == CCharacter::STATE::NEUTRAL_DOOR_ENTER)
			return NODE_STATE::NODE_RUNNING;

		// 여기에 마녀의 숲 보여주는 연출 추가하기
		/* 컷신 시작 */
		//const _uint iCurLevel = GI->Get_CurrentLevel();
		//{
		//	if (LEVELID::LEVEL_EVERMORE == iCurLevel || LEVELID::LEVEL_TOOL == iCurLevel)
		//	{
		//		CCamera_CutScene_Map* pCutSceneMap = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
		//		if (nullptr != pCutSceneMap)
		//		{
		//			pCutSceneMap->Start_CutScene(LEVELID::LEVEL_EVERMORE);
		//			pCutSceneMap->Reserve_NextCameraType(CAMERA_TYPE::FOLLOW);
		//		}
		//	}
		//}

		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest02::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_WitchForest02* CMainQuestNode_WitchForest02::Create()
{
	CMainQuestNode_WitchForest02* pInstance = new CMainQuestNode_WitchForest02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_WitchForest02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_WitchForest02::Free()
{
	__super::Free();
}

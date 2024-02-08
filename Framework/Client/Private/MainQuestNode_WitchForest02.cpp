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

#include "Camera_Group.h"

CMainQuestNode_WitchForest02::CMainQuestNode_WitchForest02()
{
}

HRESULT CMainQuestNode_WitchForest02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/06.MainQuest_WitchForest/MainQuest_WitchForest02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_WitchForest02::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_WitchForest02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* 레벨 입장 카메라 액션이 진행중이라면 리턴 */
	if (!Is_Finish_LevelEnterCameraAction())
		return NODE_STATE::NODE_RUNNING;

	if (!m_bIsStart)
	{
		m_bIsStart = true;

		/* 대화 카메라 세팅 */
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
		{
			pActionCam->Start_Action_Talk(nullptr);
		}

		/* 대화 */
		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	/* 대화가 끝나기도 전에 컷신이 재생되는 것을 방지하기 위함, 대화가 시작되고 1.5초(m_fWaitTime)가 지난 뒤에 컷신 재생 가능 */
	if (!m_bCanStartCutscene)
	{
		m_fAcc += fTimeDelta;
		if (m_fWaitTime <= m_fAcc)
			m_bCanStartCutscene = true;
	}

	if (KEY_TAP(KEY::LBTN) && m_bCanStartCutscene && !m_bStartCutscene)
	{
		m_bStartCutscene = true;

		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

		/* 컷신 시작 */
		CCamera_CutScene_Map* pCutSceneMap = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
		if (nullptr != pCutSceneMap)
		{
			pCutSceneMap->Start_CutScene(LEVELID::LEVEL_WITCHFOREST);
			pCutSceneMap->Reserve_NextCameraType(CAMERA_TYPE::ACTION);
		}
	}

	/* 컷신 재생이 끝나고 페이드까지 끝나면 다음 노드로 넘어간다. */
	if (m_bStartCutscene && CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key() == CAMERA_TYPE::ACTION && CUI_Manager::GetInstance()->Is_FadeFinished())
	{
		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}
	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_WitchForest02::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_01_01KuuSay_Intro.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}
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

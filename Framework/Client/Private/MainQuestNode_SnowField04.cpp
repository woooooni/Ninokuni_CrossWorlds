#include "stdafx.h"
#include "MainQuestNode_SnowField04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Quest_Manager.h"
#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_SnowField04::CMainQuestNode_SnowField04()
{
}

HRESULT CMainQuestNode_SnowField04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("주둔지의 지휘관 찾기");
	m_strQuestContent = TEXT("주둔지에서 지휘관 찾아보자");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("주변 몬스터 정리");
	m_strNextQuestContent = to_wstring(CQuest_Manager::GetInstance()->Get_MonsterKillCount());
	m_strNextQuestContent = m_strNextQuestContent + L" / 7";

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_SnowField04::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pJackson= GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("GrimalKinML01"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pJackson);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pJackson);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_SnowField04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			/* 대화 카메라 종료 */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField04::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField04::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_00_JacksonSay_WhatIsThis.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand03Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_01_KuuSay_Hey!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_02_JacksonSay_OhOhGotIt.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_03_KuuSay_WhatThere.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis01D"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_04_JacksonSay_Ha...ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand02Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_05_JacksonSay_First....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionEmbarrassed"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_06_KuuSay_Ok.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_04_07_JacksonSay_Thankyou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}

}

CMainQuestNode_SnowField04* CMainQuestNode_SnowField04::Create()
{
	CMainQuestNode_SnowField04* pInstance = new CMainQuestNode_SnowField04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField04::Free()
{
	__super::Free();
}

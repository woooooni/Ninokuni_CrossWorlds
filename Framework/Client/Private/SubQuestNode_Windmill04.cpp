#include "stdafx.h"
#include "SubQuestNode_Windmill04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "GameNpc.h"

CSubQuestNode_Windmill04::CSubQuestNode_Windmill04()
{
}

HRESULT CSubQuestNode_Windmill04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("풍차 수리");
	m_strQuestContent = TEXT("비어드에게 가기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("풍차 수리");
	m_strNextQuestContent = TEXT("괴짜 소녀를 찾기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Windmill04::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pBeard = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"SwiftSolutionMaster");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pBeard);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pBeard);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Windmill04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			// Windmill05로 옮겨서 해보자.
			//CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			//QuestDesc.strType = m_strNextQuestTag;
			//QuestDesc.strTitle = m_strNextQuestName;
			//QuestDesc.strContents = m_strNextQuestContent;
			//CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			/* 대화 카메라 종료 */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();

			// 엔지니어 생성.
			OBJECT_INIT_DESC pDesc = {};
			pDesc.vStartPosition = { -73.7f, -7.f, -55.27, 1.f };

			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Prorotype_GameObject_Engineer_Npc"), &pDesc)))
			{
				MSG_BOX("Fail Clone : Quest Engineer Npc");
			}

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill04::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Windmill04::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_BeardSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970011_std"));
		/* 대화 카메라 타겟 변경 (첫 대화 시작은 무조건 NPC) */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_BeardSay_ohh.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970010_std"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_Wath.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER); 
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("04_BeardSay_HaHaHa.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970003_std"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER); 
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("05_KuuSay_EEEEEE.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 6:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_NeverDont.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p010_100110_idl"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("07_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	}
}

CSubQuestNode_Windmill04* CSubQuestNode_Windmill04::Create()
{
	CSubQuestNode_Windmill04* pInstance = new CSubQuestNode_Windmill04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill04::Free()
{
	__super::Free();
}

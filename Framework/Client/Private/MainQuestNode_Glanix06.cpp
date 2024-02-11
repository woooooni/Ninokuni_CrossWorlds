#include "stdafx.h"
#include "MainQuestNode_Glanix06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_Glanix06::CMainQuestNode_Glanix06()
{
}

HRESULT CMainQuestNode_Glanix06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("장교 잭슨에게 보고하기");
	m_strQuestContent = TEXT("잭슨에게 보고하자");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("다시 에스타바니아로");
	m_strNextQuestContent = TEXT("왔던 길을 돌아가 에스타바니아로 돌아가기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix06.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Glanix06::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pJackson = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("GrimalKinML01"));

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

CBTNode::NODE_STATE CMainQuestNode_Glanix06::Tick(const _float& fTimeDelta)
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

void CMainQuestNode_Glanix06::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix06::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_06_00_KuuSay_Hey~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_06_01_JacksonSay_Ohh!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_06_02_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_06_03_JacksonSay_Great!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand03Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_06_04_KuuSay_OK!2!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHatchOut01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	}

}

CMainQuestNode_Glanix06* CMainQuestNode_Glanix06::Create()
{
	CMainQuestNode_Glanix06* pInstance = new CMainQuestNode_Glanix06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix06::Free()
{
	__super::Free();
}

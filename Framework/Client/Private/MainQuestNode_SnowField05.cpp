#include "stdafx.h"
#include "MainQuestNode_SnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"
#include "Quest_Manager.h"

CMainQuestNode_SnowField05::CMainQuestNode_SnowField05()
{
}

HRESULT CMainQuestNode_SnowField05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("주변 몬스터 정리");
	m_strQuestContent = to_wstring(CQuest_Manager::GetInstance()->Get_MonsterKillCount());
	m_strQuestContent = m_strQuestContent + L" / 7";

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("장교 잭슨에게 보고하기");
	m_strNextQuestContent = TEXT("잭슨에게 보고하자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField05.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_SnowField05::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_MONSTER_KILL);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_SnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_iPrevKillCount != CQuest_Manager::GetInstance()->Get_MonsterKillCount())
	{
		m_strQuestContent = to_wstring(CQuest_Manager::GetInstance()->Get_MonsterKillCount());
		m_strQuestContent = m_strQuestContent + L" / 7";
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strQuestTag, m_strQuestName, m_strQuestContent);
		m_iPrevKillCount = CQuest_Manager::GetInstance()->Get_QuestClearStack();
	}


	switch (CQuest_Manager::GetInstance()->Get_MonsterKillCount())
	{
	case 1:
		if (m_iTalkIndex == 0)
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			m_bIsShowDialog = true;

			TalkEvent();

			m_iTalkIndex += 1;
		}
		break;

	case 4:
		if (m_iTalkIndex == 1)
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			m_bIsShowDialog = true;

			TalkEvent();

			m_iTalkIndex += 1;
		}
		break;

	case 7:
		if (m_iTalkIndex == 2)
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			m_bIsShowDialog = true;

			TalkEvent();

			m_iTalkIndex += 1;
		}
		break;
	}

	if (m_bIsShowDialog)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= 3.f)
		{
			if (CQuest_Manager::GetInstance()->Get_MonsterKillCount() >= 7)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				CUI_PopupQuest::QUEST_INFO QuestDesc = {};
				QuestDesc.strType = m_strNextQuestTag;
				QuestDesc.strTitle = m_strNextQuestName;
				QuestDesc.strContents = m_strNextQuestContent;
				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

				CQuest_Manager::GetInstance()->Clear_MonsterKillCount();
				CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);
				
				m_bIsClear = true;

				return NODE_STATE::NODE_FAIL;
			}

			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
			m_fTime = m_fTalkChangeTime - m_fTime;
			m_bIsShowDialog = false;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField05::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField05::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_05_00_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_05_00_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_05_02_KuuSay_LetsGo~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	}

}

CMainQuestNode_SnowField05* CMainQuestNode_SnowField05::Create()
{
	CMainQuestNode_SnowField05* pInstance = new CMainQuestNode_SnowField05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField05::Free()
{
	__super::Free();
}

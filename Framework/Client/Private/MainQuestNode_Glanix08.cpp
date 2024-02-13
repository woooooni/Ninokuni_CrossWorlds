#include "stdafx.h"
#include "MainQuestNode_Glanix08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Kuu.h"

CMainQuestNode_Glanix08::CMainQuestNode_Glanix08()
{
}

HRESULT CMainQuestNode_Glanix08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("성으로");
	m_strQuestContent = TEXT("성으로 가기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("루슬란에게 보고하기");
	m_strNextQuestContent = TEXT("루슬란과 대화하기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix08.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	m_fTalkChangeTime = 3.f;

	return S_OK;
}

void CMainQuestNode_Glanix08::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_Glanix08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_EVERMORE)
	{
		if (!m_bIsStart)
		{
			if (!Is_Finish_LevelEnterCameraAction())
				return NODE_STATE::NODE_RUNNING;

			m_bIsStart = true;

			/* 대화 */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			// CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
		
		m_fTime += fTimeDelta;

		if (m_fTime >= m_fTalkChangeTime)
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("UI_Fx_Comm_Dialog_Text_1.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}

	if (GI->Get_CurrentLevel() == LEVEL_KINGDOMHALL)
	{
		// 다음 노드에서 DestSpot Position을 업데이트 한다.
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strNextQuestTag;
		QuestDesc.strTitle = m_strNextQuestName;
		QuestDesc.strContents = m_strNextQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

		CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix08::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix08::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_KuuSay_Suprise.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_08_03_08_01_KuuSay_LetsGo!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}
}

CMainQuestNode_Glanix08* CMainQuestNode_Glanix08::Create()
{
	CMainQuestNode_Glanix08* pInstance = new CMainQuestNode_Glanix08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix08::Free()
{
	__super::Free();
}
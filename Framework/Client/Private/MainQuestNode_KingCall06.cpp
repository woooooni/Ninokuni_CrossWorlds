#include "stdafx.h"
#include "MainQuestNode_KingCall06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "Sound_Manager.h"

#include "Game_Manager.h"

CMainQuestNode_KingCall06::CMainQuestNode_KingCall06()
{
}

HRESULT CMainQuestNode_KingCall06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("코에루크 설원으로");
	m_strQuestContent = TEXT("성 밖으로 나가기");

	// 여태 만든거 확인하기.
	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/02.MainQuest_KingCall/MainQuest_KingCall06.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_KingCall06::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_KingCall06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (!Is_EndCameraBlender())
		return NODE_STATE::NODE_RUNNING;

	if (!m_bIsStart)
	{
		/* 대화 */
		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
		CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();

		m_bIsStart = true;
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= 3.f)
	{
		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Dialog_Text_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

				return NODE_STATE::NODE_SUCCESS;
			}

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_KingCall06::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_KingCall06::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_06_00_KuuSay_AhHa!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_06_01_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}
}

CMainQuestNode_KingCall06* CMainQuestNode_KingCall06::Create()
{
	CMainQuestNode_KingCall06* pInstance = new CMainQuestNode_KingCall06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall06::Free()
{
	__super::Free();
}

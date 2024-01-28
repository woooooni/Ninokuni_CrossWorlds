#include "stdafx.h"
#include "MainQuestNode_WitchForest01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"

CMainQuestNode_WitchForest01::CMainQuestNode_WitchForest01()
{
}

HRESULT CMainQuestNode_WitchForest01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ������");
	m_strQuestContent = TEXT("���� ���� ���� ������ ������ ������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("���ฦ ã�Ƽ�");
	m_strNextQuestContent = TEXT("������� ��� �̶��� ã��");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/06.MainQuest_WitchForest/MainQuest_WitchForest01.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_WitchForest01::Start()
{
	CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 1);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_WitchForest01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	m_fTime += fTimeDelta;

	if (m_fTime >= 3.f)
	{
		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}


	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest01::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_WitchForest01::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_00_KuuSay_AhHa!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}
}

CMainQuestNode_WitchForest01* CMainQuestNode_WitchForest01::Create()
{
	CMainQuestNode_WitchForest01* pInstance = new CMainQuestNode_WitchForest01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_WitchForest01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_WitchForest01::Free()
{
	__super::Free();
}
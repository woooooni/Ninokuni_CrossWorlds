#include "stdafx.h"
#include "MainQuestNode_SnowField02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Player.h"

CMainQuestNode_SnowField02::CMainQuestNode_SnowField02()
{
}

HRESULT CMainQuestNode_SnowField02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("코에루크 설원으로");
	m_strQuestContent = TEXT("동쪽 문을 통해 설원으로 향하기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("코에루크 설원으로");
	m_strNextQuestContent = TEXT("동쪽 문을 통해 설원으로 향하기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_SnowField02::Start()
{
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_SnowField02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_EVERMORE)
	{
		if (!Is_Finish_LevelEnterCameraAction())
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

				if (m_iTalkIndex >= m_vecTalkDesc.size())
					CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

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
	}

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strNextQuestTag;
		QuestDesc.strTitle = m_strNextQuestName;
		QuestDesc.strContents = m_strNextQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

		m_bIsClear = true;

		CRenderer::FOG_DESC fogDesc;
		::ZeroMemory(&fogDesc, sizeof(fogDesc));
		{
			fogDesc.fFogDistanceValue = 50.0f;
			fogDesc.fFogHeightValue = 50.0f;
			fogDesc.fFogStartDepth = 100.0f;
			fogDesc.fFogStartDistance = -1.370f;
			fogDesc.fFogDistanceDensity = 0.04f;
			fogDesc.fFogHeightDensity = 0.090f;
		}
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogDesc(fogDesc);
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogColor(Vec4(0.804f, 0.871f, 0.902f, 1.0f));

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField02::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_00_KuuSay_AhHa!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}
}

CMainQuestNode_SnowField02* CMainQuestNode_SnowField02::Create()
{
	CMainQuestNode_SnowField02* pInstance = new CMainQuestNode_SnowField02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField02::Free()
{
	__super::Free();
}

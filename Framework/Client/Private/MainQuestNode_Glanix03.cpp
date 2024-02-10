#include "stdafx.h"
#include "MainQuestNode_Glanix03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_Glanix03::CMainQuestNode_Glanix03()
{
}

HRESULT CMainQuestNode_Glanix03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("더욱 더 깊게");
	m_strQuestContent = TEXT("코에루크 설원의 끝 지점까지 가기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("고대의 설인");
	m_strNextQuestContent = TEXT("기안티 처치하기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix03.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Glanix03::Start()
{
	Vec4 vSpotPos = { -44.f, 1.5f, 330.f, 1.f };
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

	vSpotPos = { -45.f, 1.6f, 298.f, 1.f };
	m_pTalkSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_Glanix03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (m_pTalkSpot != nullptr)
		{
			m_pTalkSpot->Tick(fTimeDelta);
			m_pTalkSpot->LateTick(fTimeDelta);

			if (m_pTalkSpot != nullptr)
			{
				if (m_pTalkSpot->Get_IsCol())
				{
					/* 대화 */
					m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
					m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

					CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
					CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

					TalkEvent();

					m_bIsTalkEvent = true;
					m_pTalkSpot->Set_ReadyDelete(true);
					Safe_Release(m_pTalkSpot);
				}
			}
		}


		if (m_bIsTalkEvent)
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= 3.f)
			{
				if (m_iTalkIndex < m_vecTalkDesc.size())
				{
					Safe_Delete_Array(m_szpOwner);
					Safe_Delete_Array(m_szpTalk);

					m_iTalkIndex += 1;

					if (m_iTalkIndex >= m_vecTalkDesc.size())
					{
						CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
						m_bIsTalkEvent = false;
					}

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

		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					m_bIsClear = true;
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);

					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix03::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix03::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_03_00_KuuSay_Suprise.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_03_01_KuuSay_Wait!This...ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_03_02_KuuSay_hhhhh......ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	}
}

CMainQuestNode_Glanix03* CMainQuestNode_Glanix03::Create()
{
	CMainQuestNode_Glanix03* pInstance = new CMainQuestNode_Glanix03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix03::Free()
{
	Safe_Release(m_pTalkSpot);
	__super::Free();
}

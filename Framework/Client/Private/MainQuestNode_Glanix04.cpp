#include "stdafx.h"
#include "MainQuestNode_Glanix04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"
#include "Quest_Manager.h"

#include "Glanix.h"

CMainQuestNode_Glanix04::CMainQuestNode_Glanix04()
{
}

HRESULT CMainQuestNode_Glanix04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("고대의 설인");
	m_strQuestContent = TEXT("기안티 처치하기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("장교 잭슨에게 보고하기");
	m_strNextQuestContent = TEXT("잭슨에게 보고하자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}



void CMainQuestNode_Glanix04::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"), nullptr, &m_pGlanix)))
	{
		MSG_BOX("Fail AddGameObj : Quest Glanix");
	}

	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_BOSS_KILL);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_Glanix04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pGlanix != nullptr)
	{
		// 최초 Turn~Chase 시 발동
		if (!m_bIsIntroTalk && m_pGlanix->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGlanix::GLANIX_TURN)
		{
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strQuestTag;
			QuestDesc.strTitle = m_strQuestName;
			QuestDesc.strContents = m_strQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(TEXT("더욱 더 깊게"), &QuestDesc);
			m_bIsIntroTalk = true;
		}

		if (!m_bIsRage1Talk && m_pGlanix->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGlanix::GLANIX_RAGE2WAVE)
			m_bIsRage1Talk = true;

		if (!m_bIsRage2Talk && m_pGlanix->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGlanix::GLANIX_RAGECHARGE)
			m_bIsRage2Talk = true;

		if (m_bIsTalk)
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= m_fTalkChangeTime)
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::BATTLE_DIALOG);
				m_fTime = m_fTalkChangeTime - m_fTime;
				m_iTalkIndex += 1;
				m_bIsTalk = false;
			}
		}

		BossBattle_TalkEvent(fTimeDelta);

		if (CQuest_Manager::GetInstance()->Get_IsBossKill())
		{
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

			CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);
			CQuest_Manager::GetInstance()->Set_IsBossKill(false);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix04::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix04::BossBattle_TalkEvent(const _float& fTimeDelta)
{
	switch (m_iTalkIndex)
	{
	case 0:
		if (m_bIsIntroTalk)
		{
			if (!m_bIsTalk)
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 1:
		if (m_bIsIntroTalk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsIntroTalk = false;
				m_bIsTalk = true;
			}
		}
		break;

	case 2:
		if (m_bIsRage1Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsIntroTalk = false;		
				m_bIsTalk = true;
			}
		}
		break;

	case 3:
		if (m_bIsRage1Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsIntroTalk = false;
				m_bIsTalk = true;
			}
		}
		break;

	case 4:
		if (m_bIsRage1Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsIntroTalk = false;
				m_bIsTalk = true;
			}
		}
		break;

	case 5:
		if (m_bIsRage2Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsIntroTalk = false;
				m_bIsTalk = true;
			}
		}
		break;

	case 6:
		if (m_bIsRage2Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsIntroTalk = false;
				m_bIsTalk = true;
			}
		}
		break;
	}
}

void CMainQuestNode_Glanix04::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_00_KuuSay_Akk!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_01_KuuSay_Battle!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_02_KuuSay_EEEE....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_03_04_KuuSay_AhHa!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_03_04_KuuSay_AhHa!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_05_KuuSay_WatchOut!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 6:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_06_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	}

}

CMainQuestNode_Glanix04* CMainQuestNode_Glanix04::Create()
{
	CMainQuestNode_Glanix04* pInstance = new CMainQuestNode_Glanix04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix04::Free()
{
	__super::Free();
}

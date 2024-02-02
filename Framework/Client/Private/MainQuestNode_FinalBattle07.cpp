#include "stdafx.h"
#include "MainQuestNode_FinalBattle07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Quest_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Stellia.h"
#include "DreamMazeWitch_Npc.h"

CMainQuestNode_FinalBattle07::CMainQuestNode_FinalBattle07()
{
}

HRESULT CMainQuestNode_FinalBattle07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("어둠을 먹는 검은 늑대");
	m_strQuestContent = TEXT("스텔리아 처치하기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/08.MainQuest_FinalBattle/MainQuest_FinalBattle07.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_FinalBattle07::Start()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Finish_Action_Talk();

	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_BOSS_KILL);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pStellia = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_MONSTER, TEXT("Stellia"));
	m_pStellia->Get_Component_StateMachine()->Change_State(CStellia::STELLIA_COMBATIDLE);

	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));
	m_pWitch->Get_Component_StateMachine()->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);

	if (m_pStellia == nullptr)
		MSG_BOX("FinalBattle07 : Fail Find Stellia");
	if (m_pWitch == nullptr)
		MSG_BOX("FinalBattle07 : Fail Find Witch");

}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pStellia != nullptr)
	{
		// 최초 CombatIdle 시 발동
		if (!m_bIsIntroTalk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_COMBATIDLE)
		{
			m_bIsIntroTalk = true;
		}

		if (!m_bIsRage1Talk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE1LOOP_TURN)
			m_bIsRage1Talk = true;

		if (!m_bIsRage2Talk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE2LOOP)
			m_bIsRage2Talk = true;

		if (!m_bIsRage3Talk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE3TURN_AROUND)
			m_bIsRage3Talk = true;

		if (m_bIsTalk)
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= m_fTalkChangeTime)
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 2);
				m_fTime = m_fTalkChangeTime - m_fTime;
				m_iTalkIndex += 1;
				m_bIsTalk = false;
			}
		}

		BossBattle_TalkEvent(fTimeDelta);

		if (CQuest_Manager::GetInstance()->Get_IsBossKill())
		{
			CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

			CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);
			CQuest_Manager::GetInstance()->Set_IsBossKill(false);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle07::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_FinalBattle07::BossBattle_TalkEvent(const _float& fTimeDelta)
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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

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

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 7:
		if (m_bIsRage2Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 8:
		if (m_bIsRage3Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 9:
		if (m_bIsRage3Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 2);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;
	}
}

void CMainQuestNode_FinalBattle07::TalkEvent()
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
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_04_06_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	}

}

CMainQuestNode_FinalBattle07* CMainQuestNode_FinalBattle07::Create()
{
	CMainQuestNode_FinalBattle07* pInstance = new CMainQuestNode_FinalBattle07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle07::Free()
{
	__super::Free();
}

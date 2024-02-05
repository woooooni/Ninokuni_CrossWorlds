#include "stdafx.h"
#include "SubQuestNode_Wanted06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Quest_Manager.h"

CSubQuestNode_Wanted06::CSubQuestNode_Wanted06()
{
}

HRESULT CSubQuestNode_Wanted06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("수상한 남자 잡기");
	m_strQuestContent = TEXT("녀석들을 혼내주자");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("나쁜 남자 잡기");
	m_strNextQuestContent = TEXT("녀석들을 혼내주자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/03. SubQuest03_Tumba_Wanted/SubQuest_Wanted06.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Wanted06::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_MONSTER_KILL);

	/* 현재 퀘스트에 연관있는 객체들 */
	Vec4 vSpawnPos = { -37.f, -5.3f, -44.5f, 1.f };
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Criminal_Monster"), &vSpawnPos)))
		MSG_BOX("Fail AddGameObj : Quest Criminal_Monster1");
	
	vSpawnPos = { -49.6f, -5.2f, -34.9f, 1.f };
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Criminal_Monster"), &vSpawnPos)))
		MSG_BOX("Fail AddGameObj : Quest Criminal_Monster2");
	
	vSpawnPos = { -55.8f, -5.3f, -38.4f, 1.f };
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Criminal_Monster"), &vSpawnPos)))
		MSG_BOX("Fail AddGameObj : Quest Criminal_Monster3");

	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Wanted06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
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

		if (CQuest_Manager::GetInstance()->Get_MonsterKillCount() >= 3)
		{
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

			CQuest_Manager::GetInstance()->Clear_MonsterKillCount();
			CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);

			m_bIsClear = true;
		
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted06::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Wanted06::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_05_00_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_05_00_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	}
}

CSubQuestNode_Wanted06* CSubQuestNode_Wanted06::Create()
{
	CSubQuestNode_Wanted06* pInstance = new CSubQuestNode_Wanted06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted06::Free()
{
	__super::Free();
}

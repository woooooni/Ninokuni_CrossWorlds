#include "stdafx.h"
#include "MainQuestNode_Glanix04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Quest_Manager.h"

CMainQuestNode_Glanix04::CMainQuestNode_Glanix04()
{
}

HRESULT CMainQuestNode_Glanix04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("고대의 설인");
	m_strQuestContent = TEXT("기안티 처치하기");

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
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_BOSS_KILL);

	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* 현재 퀘스트에 연관있는 객체들 */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	m_vecTalker.push_back(m_pKuu);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	// CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_Glanix04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_fTime >= 5.f)
	{
		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			// CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}

		if (m_iTalkIndex >= m_vecTalkDesc.size())
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

		m_fTime = m_fTalkChangeTime - m_fTime;
	}


	if (CQuest_Manager::GetInstance()->Get_IsBossKill())
	{
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

		CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);
		CQuest_Manager::GetInstance()->Set_IsBossKill(false);

		m_bIsClear = true;
		CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

		return NODE_STATE::NODE_FAIL;
	}


	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix04::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix04::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_ChloeSay_Introduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("01_ChloeSay_Pet.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
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

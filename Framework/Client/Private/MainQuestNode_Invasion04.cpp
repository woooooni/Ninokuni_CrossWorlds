#include "stdafx.h"
#include "MainQuestNode_Invasion04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"

CMainQuestNode_Invasion04::CMainQuestNode_Invasion04()
{
}

HRESULT CMainQuestNode_Invasion04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("다시 루슬란에게");
	m_strQuestContent = TEXT("루슬란에게 보고하자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/05.MainQuest_Invasion/MainQuest_Invasion02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Invasion04::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_Invasion04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			return NODE_STATE::NODE_SUCCESS;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion04::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion04::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_00_KuuSay_Hmm....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		//m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	}
}

CMainQuestNode_Invasion04* CMainQuestNode_Invasion04::Create()
{
	CMainQuestNode_Invasion04* pInstance = new CMainQuestNode_Invasion04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion04::Free()
{
	__super::Free();
}

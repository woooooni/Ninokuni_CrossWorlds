#include "stdafx.h"
#include "MainQuestNode_SnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

CMainQuestNode_SnowField05::CMainQuestNode_SnowField05()
{
}

HRESULT CMainQuestNode_SnowField05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("몬스터 정리");
	m_strQuestContent = TEXT("주변 몬스터의 수를 줄이자.");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField05.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_SnowField05::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	m_vecTalker.push_back(m_pKuu);

	/* 대화 */
	// m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	// m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	// 미니 다이얼로그로
	// CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	// TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_SnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// Temp
	if (KEY_TAP(KEY::N))
	{
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	// 미니 다이얼로그 연동되면 ㄱㄱ
	//if (KEY_TAP(KEY::LBTN))
	//{
	//	Safe_Delete_Array(m_szpOwner);
	//	Safe_Delete_Array(m_szpTalk);
	//
	//	m_iTalkIndex += 1;
	//
	//	if (m_iTalkIndex >= m_vecTalkDesc.size())
	//	{
	//		m_bIsClear = true;
	//		CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);
	//
	//		//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	//		//if (nullptr != pActionCam)
	//		//	pActionCam->Finish_Action_Talk();
	//
	//		return NODE_STATE::NODE_FAIL;
	//	}
	//
	//	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	//	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);
	//
	//	// 미니 다이얼로그로
	//	// CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);
	//
	//	TalkEvent();
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField05::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField05::TalkEvent()
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
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_KuuSay_I_No_Pet.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	}

}

CMainQuestNode_SnowField05* CMainQuestNode_SnowField05::Create()
{
	CMainQuestNode_SnowField05* pInstance = new CMainQuestNode_SnowField05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField05::Free()
{
	__super::Free();
}

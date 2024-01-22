#include "stdafx.h"
#include "SubQuestNode_FindCat06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "Sound_Manager.h"

#include "Game_Manager.h"

CSubQuestNode_FindCat06::CSubQuestNode_FindCat06()
{
}

HRESULT CSubQuestNode_FindCat06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("고양이 찾기");
	m_strQuestContent = TEXT("클로이의 고양이 찾기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/01. SubQuest01_Chloe_FindCat/SubQuest_FindCat06.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_FindCat06::Start()
{
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 1);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_FindCat06::Tick(const _float& fTimeDelta)
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
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);
				CQuest_Manager::GetInstance()->Set_QuestClearStack(1);

				return NODE_STATE::NODE_SUCCESS;
			}

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}


	//if (KEY_TAP(KEY::LBTN))
	//{
	//	Safe_Delete_Array(m_szpOwner);
	//	Safe_Delete_Array(m_szpTalk);
	//
	//	m_iTalkIndex += 1;
	//
	//	if (m_iTalkIndex >= m_vecTalkDesc.size())
	//	{
	//		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
	//
	//		m_bIsClear = true;
	//		CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);
	//
	//		//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	//		//if (nullptr != pActionCam)
	//		//	pActionCam->Finish_Action_Talk();
	//
	//		/* 마지막 퀘스트 노드이므로 Success 반환.*/
	//		CQuest_Manager::GetInstance()->Set_QuestClearStack(1);
	//		return NODE_STATE::NODE_SUCCESS;
	//	}
	//
	//	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	//	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);
	//
	//	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);
	//
	//	TalkEvent();
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_FindCat06::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_FindCat06::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_KuuSay_Heong.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	}
}

CSubQuestNode_FindCat06* CSubQuestNode_FindCat06::Create()
{
	CSubQuestNode_FindCat06* pInstance = new CSubQuestNode_FindCat06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_FindCat06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_FindCat06::Free()
{
	__super::Free();
}

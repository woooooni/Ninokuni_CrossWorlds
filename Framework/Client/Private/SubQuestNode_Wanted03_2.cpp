#include "stdafx.h"
#include "SubQuestNode_Wanted03_2.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"
#include "Criminal_Npc.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

CSubQuestNode_Wanted03_2::CSubQuestNode_Wanted03_2()
{
}

HRESULT CSubQuestNode_Wanted03_2::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("수상한 남자 잡기");
	m_strQuestContent = TEXT("수상해 보이는 남자 찾기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("수상한 남자 잡기");
	m_strNextQuestContent = TEXT("수상한 남자 추격하기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/03. SubQuest03_Tumba_Wanted/SubQuest_Wanted03_2.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Wanted03_2::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pCriminal = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, L"Criminal");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pCriminal);

	/* 대화 카메라 세팅 */
	{
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_IDLE);
		
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
			pFollowCam->Reset_WideView_To_DefaultView(false);
	}


	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Wanted03_2::Tick(const _float& fTimeDelta)
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
			m_bIsClear = true;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			/* 대화 카메라 종료 */
			{
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);

				if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
					CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
			}
			m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCriminal_Npc::NPC_CRIMINAL_STATE::NPC_CRIMINAL_ESCAPE);

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted03_2::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Wanted03_2::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_TumbaSay_Call.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Uh.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_TumbaSay_OkCallYou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand04Idle01"));
		break;
	case 3:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_WhatIsBird.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 4:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("04_TumbaSay_Umm..This....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		break;
	}
}

CSubQuestNode_Wanted03_2* CSubQuestNode_Wanted03_2::Create()
{
	CSubQuestNode_Wanted03_2* pInstance = new CSubQuestNode_Wanted03_2();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted03_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted03_2::Free()
{
	__super::Free();
}

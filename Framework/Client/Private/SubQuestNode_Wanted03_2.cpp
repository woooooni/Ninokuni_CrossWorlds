#include "stdafx.h"
#include "SubQuestNode_Wanted03_2.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"
#include "Criminal_Npc.h"

#include "Camera_Group.h"

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

	m_fSkipTime = 1.f;

	return S_OK;
}

void CSubQuestNode_Wanted03_2::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pCriminal = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, L"Criminal");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pCriminal);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
	{
		m_vCirminalOriginLookAt = m_pCriminal->Get_Component_Transform()->Get_LookAt();
		pActionCam->Set_TalkBackupDesc(m_pCriminal->Get_Component_Transform());

		m_pCriminal->Get_Component_Transform()->LookAt_ForLandObject((m_vCirminalOriginLookAt * -1.f).OneW());
		pActionCam->Start_Action_Talk(m_pCriminal);

		pActionCam->Set_NpcTransformByBackupDesc(m_pCriminal->Get_Component_Transform());
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

	// 일반 대화
	if (KEY_TAP(KEY::LBTN) && m_iTalkIndex < m_vecTalkDesc.size() - 1)
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		if (m_iTalkIndex >= m_vecTalkDesc.size() - 1)
		{
			m_pCriminal->Get_Component_StateMachine()->Change_State(CCriminal_Npc::NPC_CRIMINAL_STATE::NPC_CRIMINAL_ESCAPE);
		}

		if (m_iTalkIndex < m_vecTalkDesc.size() - 1)
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	// 어 이 자식 튄다!
	if (m_iTalkIndex == m_vecTalkDesc.size() - 1 && !m_bIsSkipEvent &&
		m_pCriminal->Get_Component_StateMachine()->Get_CurrState() == CCriminal_Npc::NPC_CRIMINAL_STATE::NPC_CRIMINAL_ESCAPERUN)
	{
		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();

		m_bIsSkipEvent = true;
	}

	// 넘어가기 전 1.5초 정도 텀
	if (m_bIsSkipEvent)
	{
		m_fAccSkipTime += fTimeDelta;
		if (m_fAccSkipTime >= m_fSkipTime)
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_bIsClear = true;
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			/* 대화 카메라 종료 */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();

			return NODE_STATE::NODE_FAIL;
		}
	}
	

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted03_2::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Wanted03_2::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Wanted_03_02_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));

		// 아래 카메라 액션 변경시 카메라 담당 상의 필요
		m_pCriminal->Get_Component_Transform()->LookAt_ForLandObject((m_vCirminalOriginLookAt * -1.f).OneW());
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		pActionCam->Set_NpcTransformByBackupDesc(m_pCriminal->Get_Component_Transform());
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Wanted_03_02_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));

		// 아래 카메라 액션 변경시 카메라 담당 상의 필요
		m_pCriminal->Get_Component_Transform()->LookAt_ForLandObject((m_vCirminalOriginLookAt * -1.f).OneW());
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		pActionCam->Set_NpcTransformByBackupDesc(m_pCriminal->Get_Component_Transform());
		break;
	case 2:
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand04Idle01"));

		// 아래 카메라 액션 변경시 카메라 담당 상의 필요
		m_pCriminal->Get_Component_Transform()->LookAt_ForLandObject((m_vCirminalOriginLookAt * -1.f).OneW());
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		pActionCam->Set_NpcTransformByBackupDesc(m_pCriminal->Get_Component_Transform());
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Wanted_03_02_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));

		// 아래 카메라 액션 변경시 카메라 담당 상의 필요
		m_pCriminal->Get_Component_Transform()->LookAt_ForLandObject((m_vCirminalOriginLookAt * -1.f).OneW());
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		pActionCam->Set_NpcTransformByBackupDesc(m_pCriminal->Get_Component_Transform());
		break;
	case 4:
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));

		// 아래 카메라 액션 변경시 카메라 담당 상의 필요
		m_pCriminal->Get_Component_Transform()->LookAt_ForLandObject((m_vCirminalOriginLookAt * -1.f).OneW());
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		pActionCam->Set_NpcTransformByBackupDesc(m_pCriminal->Get_Component_Transform());
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Wanted_03_02_06.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
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

#include "stdafx.h"
#include "MainQuestNode_PlantKiller02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_PlantKiller02::CMainQuestNode_PlantKiller02()
{
}

HRESULT CMainQuestNode_PlantKiller02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/07.MainQuest_PlantKiller/MainQuest_PlantKiller02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_PlantKiller02::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pCyan = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, L"Cyan");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pCyan);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pCyan);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_PlantKiller02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			/* 대화 카메라 종료 */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_PlantKiller02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_PlantKiller02::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Hey.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Hello03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_VerdeSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CatchEvent_Stand"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_WeIntroduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk05"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 3:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("04_VerdeSay_AhHa.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 4:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("04_VerdeSay_AhHa.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 5:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;
	case 6:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("08_VerdeSay_Request.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk05"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 7:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHatchOut01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 8:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("10_VerdeSay_Happy1.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 9:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("12_KuuSay_Go.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk06"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 10:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("12_KuuSay_Go.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 11:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("12_KuuSay_Go.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}

}

CMainQuestNode_PlantKiller02* CMainQuestNode_PlantKiller02::Create()
{
	CMainQuestNode_PlantKiller02* pInstance = new CMainQuestNode_PlantKiller02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller02::Free()
{
	__super::Free();
}

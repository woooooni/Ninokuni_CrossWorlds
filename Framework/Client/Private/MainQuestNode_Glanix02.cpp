#include "stdafx.h"
#include "MainQuestNode_Glanix02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_Glanix02::CMainQuestNode_Glanix02()
{
}

HRESULT CMainQuestNode_Glanix02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("더 깊숙히 조사하기");
	m_strQuestContent = TEXT("코에루크 설원 깊은 곳 까지 들어가보자");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("더욱 더 깊게");
	m_strNextQuestContent = TEXT("코에루크 설원의 끝 지점까지 가기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Glanix02::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pAren = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("Aren"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pAren);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pAren);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_Glanix02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN) && !m_bIsFadeOut)
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
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

			m_bIsFadeOut = true;
		}

		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	if (!m_bIsFadeIn && m_bIsFadeOut && Is_EndCameraBlender())
	{
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);

		m_bIsFadeIn = true;
	}

	if (m_bIsFadeIn)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);

			m_bIsClear = true;
			m_pAren->Set_Dead(true);

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix02::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_00_KuuSay_Hey~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_01_ArenSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pAren->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pAren->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Idle02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_02_KuuSay_Introduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_03_ArenSay_Introduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pAren->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pAren->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Hello"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_04_KuuSay_Heong!!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_05_ArenSay_Ha....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pAren->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pAren->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_06_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSurprise02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_07_ArenSay_Ha..2.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pAren->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pAren->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_08_KuuSay_Hmm!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_NeutralStand05"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 9:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_09_ArenSay_Ok.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pAren->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pAren->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;
	case 10:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_10_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionWorry"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 11:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Glanix_02_11.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pAren->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pAren->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;

	}

}

CMainQuestNode_Glanix02* CMainQuestNode_Glanix02::Create()
{
	CMainQuestNode_Glanix02* pInstance = new CMainQuestNode_Glanix02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix02::Free()
{
	__super::Free();
}

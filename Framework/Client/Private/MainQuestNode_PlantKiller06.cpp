#include "stdafx.h"
#include "MainQuestNode_PlantKiller06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"
#include "Quest_Manager.h"

CMainQuestNode_PlantKiller06::CMainQuestNode_PlantKiller06()
{
}

HRESULT CMainQuestNode_PlantKiller06::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/07.MainQuest_PlantKiller/MainQuest_PlantKiller06.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_PlantKiller06::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::GetInstance()->QUESTEVENT_RUBY_DEFENCE);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pRuby = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, L"Ruby");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pRuby);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pRuby);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_PlantKiller06::Tick(const _float& fTimeDelta)
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
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

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

void CMainQuestNode_PlantKiller06::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_PlantKiller06::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("PlantKiller_06_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pRuby->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pRuby->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Ruby.ao|Ruby_Talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("PlantKiller_06_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("PlantKiller_06_03.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pRuby->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pRuby->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Ruby.ao|Ruby_Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	}

}

CMainQuestNode_PlantKiller06* CMainQuestNode_PlantKiller06::Create()
{
	CMainQuestNode_PlantKiller06* pInstance = new CMainQuestNode_PlantKiller06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller06::Free()
{
	__super::Free();
}

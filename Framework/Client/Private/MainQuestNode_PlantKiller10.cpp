#include "stdafx.h"
#include "MainQuestNode_PlantKiller10.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_PlantKiller10::CMainQuestNode_PlantKiller10()
{
}

HRESULT CMainQuestNode_PlantKiller10::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/07.MainQuest_PlantKiller/MainQuest_PlantKiller10.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_PlantKiller10::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

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

CBTNode::NODE_STATE CMainQuestNode_PlantKiller10::Tick(const _float& fTimeDelta)
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
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 2.f);
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

	if (!m_bIsFadeIn && m_bIsFadeOut)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 2.f);
			m_bIsFadeIn = true;
		}
	}

	if (m_bIsFadeIn)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_PlantKiller10::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_PlantKiller10::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("PlantKiller_10_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("PlantKiller_10_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis01E"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("PlantKiller_10_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCyan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCyan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Cyan.ao|NpcFat_Stand01Talk06"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	}

}

CMainQuestNode_PlantKiller10* CMainQuestNode_PlantKiller10::Create()
{
	CMainQuestNode_PlantKiller10* pInstance = new CMainQuestNode_PlantKiller10();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller10");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller10::Free()
{
	__super::Free();
}

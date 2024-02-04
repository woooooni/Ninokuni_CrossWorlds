#include "stdafx.h"
#include "SubQuestNode_NoisySnowField04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "CurlingGame_Manager.h"

CSubQuestNode_NoisySnowField04::CSubQuestNode_NoisySnowField04()
{
}

HRESULT CSubQuestNode_NoisySnowField04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("시끄러운 코에루코 설원");
	m_strQuestContent = TEXT("시끄러운 남자를 찾아보기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("시끄러운 코에루코 설원");
	m_strNextQuestContent = TEXT("대결에서 승리하여 진정시키자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_NoisySnowField04::Start()
{
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pNoisyMan = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"Destroyer_Dummy");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pNoisyMan);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pNoisyMan);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

				// 1. 경기장 비춰주는 카메라 액션을 시작한다.
				if (!m_bCameraAction)
				{
					m_bCameraAction = true;
					CCurlingGame_Manager::GetInstance()->Ready_Game();
				}
			}

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}
		}

		// 2.경기장 비춰주는 카메라 액션이 종료되었다면 다음 노드로 넘어간다.
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
		{
			if (m_bCameraAction && CCamera_Action::CAMERA_ACTION_TYPE::STADIUM != pActionCam->Get_Camera_ActionType())
			{
				m_bIsClear = true;

				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField04::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField04::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_TumbaSay_Call.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Uh.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_TumbaSay_OkCallYou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand04Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 3:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_WhatIsBird.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSEndOfDonDonKing01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 4:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("04_TumbaSay_Umm..This....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_IDLE);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_CSNeutralStand"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 5:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("05_KuuSay_WhatIsCallMe.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 6:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("06_TumbaSay_Ah....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 7:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("07_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionAnnoyed"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 8:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_NeverDont.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_CostumeMotion01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 9:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("09_KuuSay_OK.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 10:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_NeverDont.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;

	}

}

CSubQuestNode_NoisySnowField04* CSubQuestNode_NoisySnowField04::Create()
{
	CSubQuestNode_NoisySnowField04* pInstance = new CSubQuestNode_NoisySnowField04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField04::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "SubQuestNode_NoisySnowField08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CSubQuestNode_NoisySnowField08::CSubQuestNode_NoisySnowField08()
{
}

HRESULT CSubQuestNode_NoisySnowField08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("시끄러운 코에루코 설원");
	m_strQuestContent = TEXT("벨라에게 돌아가기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField08.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_NoisySnowField08::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pBella = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"AquarisBella");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pBella);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pBella);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (!m_bIsRewarding)
		{
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
					CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
					CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

					/* 대화 카메라 종료 */
					CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
					if (nullptr != pActionCam)
						pActionCam->Finish_Action_Talk();

					/* 여기서 퀘스트 보상 받기.(퀘스트 보상 다 받으면 return하기.*/
					CUI_Manager::GetInstance()->OnOff_QuestRewards(true, TEXT("시끄러운 코에루코 설원"));
					m_bIsRewarding = true;
				}

				if (!m_bIsRewarding)
				{
					m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
					m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

					CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

					TalkEvent();
				}
			}
		}
		else if (m_bIsRewarding)
		{
			if (CUI_Manager::GetInstance()->Is_QuestRewardWindowOff())
			{
				m_bIsClear = true;
				return NODE_STATE::NODE_FAIL;
			}
		}
	}



	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField08::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField08::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_08_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_08_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_08_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_TurnL"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 3:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_VerdeSay_Reward.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_idle"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_08_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHatchOut01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}

}

CSubQuestNode_NoisySnowField08* CSubQuestNode_NoisySnowField08::Create()
{
	CSubQuestNode_NoisySnowField08* pInstance = new CSubQuestNode_NoisySnowField08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField08::Free()
{
	__super::Free();
}

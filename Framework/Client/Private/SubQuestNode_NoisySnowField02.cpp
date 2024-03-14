#include "stdafx.h"
#include "SubQuestNode_NoisySnowField02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CSubQuestNode_NoisySnowField02::CSubQuestNode_NoisySnowField02()
{
}

HRESULT CSubQuestNode_NoisySnowField02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_NoisySnowField02::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pBella = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"AquarisBella");

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

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField02::Tick(const _float& fTimeDelta)
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
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			// 노이즈맨 한 번만 생성.
			if (m_iTalkIndex == 11)
			{
				if (m_pNoisyMan == nullptr)
				{
					OBJECT_INIT_DESC NpcDesc = {};
					NpcDesc.vStartPosition = { -94.f, -3.f, 218.f, 1.f };

					if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_Destroyer_Npc"), &NpcDesc, &m_pNoisyMan)))
						MSG_BOX("Retry Add_GameObject Fail : m_pNoisyMan");
				}
			}

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
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField02::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField02::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_Talk1"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_03.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_05.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_06.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSilusAndMechaRagon02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_07.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_Talk1"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 8:
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 9:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_09.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSurprise03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 10:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_10.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;
	case 11:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_11.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_CSAquaris03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 12:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_12.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSilusAndMechaRagon02"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 13:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_02_13.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_AquarisBella.ao|AquarisBella_talk"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	}

}

CSubQuestNode_NoisySnowField02* CSubQuestNode_NoisySnowField02::Create()
{
	CSubQuestNode_NoisySnowField02* pInstance = new CSubQuestNode_NoisySnowField02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField02::Free()
{
	__super::Free();
}

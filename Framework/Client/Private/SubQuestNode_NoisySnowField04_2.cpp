#include "stdafx.h"
#include "SubQuestNode_NoisySnowField04_2.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "CurlingGame_Manager.h"

CSubQuestNode_NoisySnowField04_2::CSubQuestNode_NoisySnowField04_2()
{
}

HRESULT CSubQuestNode_NoisySnowField04_2::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField04_2.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_NoisySnowField04_2::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pNoisyMan = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"Destroyer_Dummy");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pNoisyMan);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField04_2::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (KEY_TAP(KEY::LBTN) && !m_bFadeOut)
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

				// 대화가 종료되었다면 페이드 아웃 시작 
				if (!m_bFadeOut)
				{
					CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 2.f);
					m_bFadeOut = true;
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

		// 페이드 아웃이 끝났다면 다음 노드로 넘어간다.
		if (m_bFadeOut)
		{
			if (CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				m_bIsClear = true;
				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField04_2::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField04_2::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_02_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_TumbaSay_Call.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_02_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	default :
		break;
	}

}

CSubQuestNode_NoisySnowField04_2* CSubQuestNode_NoisySnowField04_2::Create()
{
	CSubQuestNode_NoisySnowField04_2* pInstance = new CSubQuestNode_NoisySnowField04_2();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField04_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField04_2::Free()
{
	__super::Free();
}

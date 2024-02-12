#include "stdafx.h"
#include "MainQuestNode_Invasion01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"
#include "SkyDome.h"

#include "Camera_Group.h"

CMainQuestNode_Invasion01::CMainQuestNode_Invasion01()
{
}

HRESULT CMainQuestNode_Invasion01::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/05.MainQuest_Invasion/MainQuest_Invasion01.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Invasion01::Start()
{
	// Change SkyDomeColor

	
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_KINGDOMHALL, LAYER_NPC, TEXT("Ruslan"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pRuslan);
}

CBTNode::NODE_STATE CMainQuestNode_Invasion01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_KINGDOMHALL)
	{
		if (!m_bIsStart)
		{
			CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::GetInstance()->QUESTEVENT_INVASION);

			/* 레벨 입장 카메라 액션이 진행중이라면 리턴 */
			if(!Is_Finish_LevelEnterCameraAction())
				return NODE_STATE::NODE_RUNNING;
		
			GI->Stop_Sound(CHANNELID::SOUND_BGM_CURR, 0.f);
			GI->Play_Sound(L"Impact_Metal_Stone_1.mp3", CHANNELID::SOUND_CUTSCENE, 1.f, true);

			/* 대화 */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			/* 카메라 세팅 */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
			{
				pActionCam->Start_Action_Talk(nullptr);
				CCamera_Manager::GetInstance()->Start_Action_Shake(0.1f, 17.f, 0.3f); // 대화 카메라에 쉐이킹 적용 예정
			}

			TalkEvent();


			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RenderSwitch(CRenderer::RENDER_SWITCH::GODRAY_SWITCH, false);
			m_bIsStart = true;
		}

		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				m_bIsClear = true;
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

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

void CMainQuestNode_Invasion01::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion01::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Invasion_01_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Invasion_01_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Invasion_01_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}
}

CMainQuestNode_Invasion01* CMainQuestNode_Invasion01::Create()
{
	CMainQuestNode_Invasion01* pInstance = new CMainQuestNode_Invasion01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion01::Free()
{
	__super::Free();
}

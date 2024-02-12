#include "stdafx.h"
#include "MainQuestNode_Ending02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Game_Manager.h"

#include "Player.h"
#include "Character.h"
#include "Kuu.h"

CMainQuestNode_Ending02::CMainQuestNode_Ending02()
{
}

HRESULT CMainQuestNode_Ending02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/09.MainQuest_Ending/MainQuest_Ending02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Ending02::Start()
{	
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	CTransform* pPlayerTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CharacterTransformCom();
	CPhysX_Controller* pPlayerController = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"));

	pPlayerTransform->Set_State(CTransform::STATE_POSITION, Vec4(124.f, 0.f, 111.f, 1.f));
	pPlayerTransform->FixRotation(0.f, -90.f, 0.f);
	pPlayerController->Set_EnterLevel_Position(pPlayerTransform->Get_Position());

	/* 대화 카메라를 미리 세팅 해둔다. */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(nullptr);

	/* 컷신 시작 */
	CCamera_CutScene_Map* pCutSceneMap = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
	if (nullptr != pCutSceneMap)
	{
		pCutSceneMap->Start_CutScene(LEVELID::LEVEL_EVERMORE, 2);
		pCutSceneMap->Reserve_NextCameraType(CAMERA_TYPE::ACTION);
	}
}

CBTNode::NODE_STATE CMainQuestNode_Ending02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		CCamera_CutScene_Map* pCutSceneMapCam = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	
		/* 예약된 마지막 컷신에서, 남은 재생 시간이 1초 이하라면 페이드 아웃을 시작한다. */
		if (nullptr != pCutSceneMapCam && pCutSceneMapCam->Is_LastCutScene() && pCutSceneMapCam->Get_RemainDuration() <= m_fFadeDuration * 0.5f && !m_bFadeOut)
		{
			m_bFadeOut = true;
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_fFadeDuration, true);
			return NODE_STATE::NODE_RUNNING;
		}
		
		/* 페이드 아웃이 끝났다면 페이드 인을 바로 시작한다. 동시에 대화 카메라에서의 캐릭터들의 트랜스폼을 설정한다. */
		if (m_bFadeOut && !m_bFadeIn && CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
			{
				pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
			}

			m_bFadeIn = true;
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_fFadeDuration, true);
			return NODE_STATE::NODE_RUNNING;
		}

		/* 페이드 인이 끝났는지 체크한다. */
		if (!m_bFinishIntro)
		{
			if (m_bFadeOut && m_bFadeIn && !CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				return NODE_STATE::NODE_RUNNING;
			}
			else if (m_bFadeOut && m_bFadeIn && CUI_Manager::GetInstance()->Is_FadeFinished())
				m_bFinishIntro = true;
		}
		
		/* 페이드 인이 끝났고, 토크를 시작하지 않았다면 */
		if (m_bFinishIntro && !m_bIsTalkStart)
		{
			/* 대화 */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_bIsTalkStart = true;
		}

		if (m_bIsTalkStart)
		{
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
		}
	}


	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Ending02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Ending02::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Ending_02_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSilusAndMechaRagon02"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Ending_02_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Ending_02_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionWorry"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Ending_02_03.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Ending_02_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHatchOut01"));
		break;
	}
}

CMainQuestNode_Ending02* CMainQuestNode_Ending02::Create()
{
	CMainQuestNode_Ending02* pInstance = new CMainQuestNode_Ending02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Ending02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Ending02::Free()
{
	__super::Free();
}
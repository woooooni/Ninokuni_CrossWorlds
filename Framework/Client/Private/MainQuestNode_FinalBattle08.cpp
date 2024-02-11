#include "stdafx.h"
#include "MainQuestNode_FinalBattle08.h"

#include "GameInstance.h"
#include "Character.h"
#include "Player.h"

#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

#include "Camera_Group.h"

#include "DreamMazeWitch_Npc.h"

CMainQuestNode_FinalBattle08::CMainQuestNode_FinalBattle08()
{
}

HRESULT CMainQuestNode_FinalBattle08::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/08.MainQuest_FinalBattle/MainQuest_FinalBattle08.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_FinalBattle08::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

	if (m_pWitch != nullptr)
	{
		// 스텔리아 죽고 나서, 마녀 도망가는 애니메이션 
		m_pWitch->Get_Component_Model()->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Death"));

		/* 카메라 락온 해제 */
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && pFollowCam->Is_LockOn() && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Reset_WideView_To_DefaultView(true);
			pFollowCam->Set_Default_Position();
			pFollowCam->Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		}
		
		/* 마녀 도망가는 카메라 액션 실행 */
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
		{
			pActionCam->Start_Action_Witch_Away(m_pWitch);
			CCamera_Manager::GetInstance()->Set_CurCamera(pActionCam->Get_Key());

			/* Ui, Input */
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);
		}
	}
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (!m_bIsTalk)
		{
			/* 대화 */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
			m_bIsTalk = true;
		}
		else
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= 3.f)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
				m_bIsWitchEscape = true;
			}
		}

		m_bStartBlend;

		if (m_bIsWitchEscape)
		{
			/* 블렌딩 시작 조건 */
			if (m_pWitch->Get_Component_Model()->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Death") &&
				0.9f <= m_pWitch->Get_Component_Model()->Get_Progress() && !m_pWitch->Get_Component_Model()->Is_Tween())
			{
				if (!m_bStartBlend)
				{
					m_bStartBlend = true;

					// 카메라 블렌딩 시작 
					CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
					if (nullptr != pFollowCam)
					{
						pFollowCam->Reset_WideView_To_DefaultView(true);
						pFollowCam->Set_Default_Position();
						CCamera_Manager::GetInstance()->Change_Camera(CAMERA_TYPE::FOLLOW);
					}
				}
			}

			/* 블렌딩 완료시 다음 노드로 */
			if (m_bStartBlend && !CCamera_Manager::GetInstance()->Is_Blending_Camera())
			{
				m_bIsClear = true;

				/* Ui, Input */
				CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle08::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_FinalBattle08::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_05_00_KuuSay_Happy1.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pWitch->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	}
}

CMainQuestNode_FinalBattle08* CMainQuestNode_FinalBattle08::Create()
{
	CMainQuestNode_FinalBattle08* pInstance = new CMainQuestNode_FinalBattle08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle08::Free()
{
	__super::Free();
}

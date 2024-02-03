#include "stdafx.h"
#include "MainQuestNode_Ending02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

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

	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CharacterStateCom()->Change_State(CCharacter::STATE::NEUTRAL_DOOR_ENTER);
}

CBTNode::NODE_STATE CMainQuestNode_Ending02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CurrentState() == CCharacter::STATE::NEUTRAL_DOOR_ENTER)
			return NODE_STATE::NODE_RUNNING;

		if (!m_bIsTalkStart)
		{
			/* 대화 카메라 세팅 */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Start_Action_Talk(nullptr);

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
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_KuuSay_Suprise.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSilusAndMechaRagon02"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_08_03_08_01_KuuSay_LetsGo!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_08_03_08_01_KuuSay_LetsGo!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis02E"));
		break;
	case 3:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_08_03_08_01_KuuSay_LetsGo!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 4:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_08_03_08_01_KuuSay_LetsGo!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
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
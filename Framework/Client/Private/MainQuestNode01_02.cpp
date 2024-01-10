#include "stdafx.h"
#include "MainQuestNode01_02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Sound_Manager.h"
#include "Camera_Action.h"

CMainQuestNode01_02::CMainQuestNode01_02()
{
}

HRESULT CMainQuestNode01_02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest01/MainQuest01_02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	// string strLoad = Load.dump();

	return S_OK;
}

void CMainQuestNode01_02::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");

	/* 카메라 타겟 세팅 */
	// CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");

	//if (nullptr != pTarget)
	//{
	
		// 임시 주석
		//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		//if (nullptr != pActionCam)
		//{
		//	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);
		//	pActionCam->Start_Action_Talk(); //쿠우 혼자면 null
		//}
		
		
	//}

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode01_02::Tick(const _float& fTimeDelta)
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
			
			//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			//if (nullptr != pActionCam)
			//	pActionCam->Finish_Action_Talk();

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode01_02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode01_02::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
		// 대화 상태로 만들어서 매개로 애니메이션 이름 던지자. 답이 없다.
		// Talk, Idle, Run, Walk. 
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("KuuSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	case 2: 
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionPositive"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive"));
		break;
	case 3:
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionWorry"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionWorry"));
		break;
	case 4:
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_talk01"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 5:
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		break;
	}
}

CMainQuestNode01_02* CMainQuestNode01_02::Create()
{
	CMainQuestNode01_02* pInstance = new CMainQuestNode01_02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode01_02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode01_02::Free()
{
	__super::Free();
}

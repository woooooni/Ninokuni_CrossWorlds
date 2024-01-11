#include "stdafx.h"
#include "MainQuestNode_KingCall02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"


CMainQuestNode_KingCall02::CMainQuestNode_KingCall02()
{
}

HRESULT CMainQuestNode_KingCall02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("장교의 시선");
	m_strQuestContent = TEXT("아까부터 나를 지켜본다");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/02.MainQuest_KingCall/MainQuest_KingCall02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_KingCall02::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");
	m_pSanson = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("GrimalKinML02"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pSanson);

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

CBTNode::NODE_STATE CMainQuestNode_KingCall02::Tick(const _float& fTimeDelta)
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

void CMainQuestNode_KingCall02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_KingCall02::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_ChloeSay_Introduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_ChloeSay_Pet.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_KuuSay_I_No_Pet.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_ImKuu.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionAnnoyed"));
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("04_ChloeSay_101010_70_030.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionEmbarrassed"));
		break;
	}

}

CMainQuestNode_KingCall02* CMainQuestNode_KingCall02::Create()
{
	CMainQuestNode_KingCall02* pInstance = new CMainQuestNode_KingCall02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall02::Free()
{
	__super::Free();
}

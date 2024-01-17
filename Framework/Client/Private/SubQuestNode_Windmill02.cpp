#include "stdafx.h"
#include "SubQuestNode_Windmill02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

CSubQuestNode_Windmill02::CSubQuestNode_Windmill02()
{
}

HRESULT CSubQuestNode_Windmill02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Windmill02::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pVerde = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Verde");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pVerde);

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

CBTNode::NODE_STATE CSubQuestNode_Windmill02::Tick(const _float& fTimeDelta)
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

void CSubQuestNode_Windmill02::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Windmill02::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Hey.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis01E"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_VerdeSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pVerde->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pVerde->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_tlk"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_WeIntroduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis02D"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("04_VerdeSay_AhHa.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pVerde->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pVerde->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_TurnL"));
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("05_VerdeSay_GoodTiming.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 5:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Kuu_CSKuromiParty01"));
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("08_VerdeSay_Request.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pVerde->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pVerde->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_idl"));
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionWorry"));
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("10_VerdeSay_Happy1.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pVerde->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pVerde->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_tlk"));
		break;
	case 9:
		break;
	case 10:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("12_KuuSay_Go.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	}

}

CSubQuestNode_Windmill02* CSubQuestNode_Windmill02::Create()
{
	CSubQuestNode_Windmill02* pInstance = new CSubQuestNode_Windmill02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill02::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "SubQuestNode_FindCat02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"


CSubQuestNode_FindCat02::CSubQuestNode_FindCat02()
{
}

HRESULT CSubQuestNode_FindCat02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/01. SubQuest01_Chloe_FindCat/SubQuest_FindCat02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_FindCat02::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");
	m_pChloe= GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Chloe");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pChloe);

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

CBTNode::NODE_STATE CSubQuestNode_FindCat02::Tick(const _float& fTimeDelta)
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

void CSubQuestNode_FindCat02::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_FindCat02::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_ChloeSay_Introduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_ChloeSay_Pet.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionPositive"));
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
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionEmbarrassed"));
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("05_ChloeSay_Request.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionWorry"));
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_NeverDont.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("07_ChloeSay_KuuShy.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionNegative"));
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("08_ChloeSay_KuuWant.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionTalk"));
		break;
	case 9:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("09_KuuSay_Dia_SouthernHeartsland_101070_30_020_Kuu.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionNegative"));
		break;
	case 10:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("10_ChloeSay_ThankYou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pChloe->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_IDLE);
		//m_pChloe->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_EmotionHello"));
		break;
	}

}

CSubQuestNode_FindCat02* CSubQuestNode_FindCat02::Create()
{
	CSubQuestNode_FindCat02* pInstance = new CSubQuestNode_FindCat02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_FindCat02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_FindCat02::Free()
{
	__super::Free();
}

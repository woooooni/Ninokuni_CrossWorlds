#include "stdafx.h"
#include "MainQuestNode_Invasion01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"

CMainQuestNode_Invasion01::CMainQuestNode_Invasion01()
{
}

HRESULT CMainQuestNode_Invasion01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("성 밖의 소란");
	m_strQuestContent = TEXT("성 밖이 소란스럽다");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("성 밖의 소란");
	m_strNextQuestContent = TEXT("성 밖이 소란스럽다");


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
	/* 현재 퀘스트에 연관있는 객체들 */
	CCamera_Manager::GetInstance()->Start_Action_Shake(0.1f, 17.f, 0.5f);

	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("Ruslan"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_Invasion01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_KINGDOMHALL)
	{
		if (!m_bIsStart)
		{
			if (true == CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Shake())
				return NODE_STATE::NODE_FAIL;

			CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

			/* 대화 */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_bIsStart = true;
		}

		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

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
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion01::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion01::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_00_KuuSay_Hmm....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		//m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
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

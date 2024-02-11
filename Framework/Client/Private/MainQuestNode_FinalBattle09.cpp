#include "stdafx.h"
#include "MainQuestNode_FinalBattle09.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

CMainQuestNode_FinalBattle09::CMainQuestNode_FinalBattle09()
{
}

HRESULT CMainQuestNode_FinalBattle09::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/08.MainQuest_FinalBattle/MainQuest_FinalBattle09.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_FinalBattle09::Start()
{
	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle09::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_WITCHFOREST)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= 3.f)
		{
			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_iTalkIndex += 1;
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("UI_Fx_Comm_Dialog_Text_1.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

				if (m_iTalkIndex >= m_vecTalkDesc.size())
				{
					CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
					m_bIsClear = true;

					return NODE_STATE::NODE_SUCCESS;
				}

				if (m_iTalkIndex < m_vecTalkDesc.size())
				{
					m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
					m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

					CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

					TalkEvent();
				}

				m_fTime = m_fTalkChangeTime - m_fTime;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle09::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_FinalBattle09::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_05_00_KuuSay_Happy1.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_05_01_KuuSay_Happy2.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_05_01_KuuSay_Happy2.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	}
}

CMainQuestNode_FinalBattle09* CMainQuestNode_FinalBattle09::Create()
{
	CMainQuestNode_FinalBattle09* pInstance = new CMainQuestNode_FinalBattle09();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle09");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle09::Free()
{
	__super::Free();
}

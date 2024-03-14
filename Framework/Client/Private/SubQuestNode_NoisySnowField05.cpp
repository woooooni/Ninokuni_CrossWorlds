#include "stdafx.h"
#include "SubQuestNode_NoisySnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"

#include "CurlingGame_Group.h"

CSubQuestNode_NoisySnowField05::CSubQuestNode_NoisySnowField05()
{
}

HRESULT CSubQuestNode_NoisySnowField05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("시끄러운 코에루코 설원");
	m_strQuestContent = TEXT("대결에서 승리하여 진정시키자");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("시끄러운 코에루코 설원");
	m_strNextQuestContent = TEXT("대결에서 승리하여 진정시키자");

	return S_OK;
}

void CSubQuestNode_NoisySnowField05::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strNextQuestTag;
	QuestDesc.strTitle = m_strNextQuestName;
	QuestDesc.strContents = m_strNextQuestContent;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

	// 게임 시작 
	CCurlingGame_Manager::GetInstance()->Start_Game();

	// 페이드 인
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 2.f);
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;
	
	// 게임이 종료된후 페이드 아웃 인(컬링 게임 매니저에서 처리)까지 끝난 상태라면 다음 노드로 넘어간다. 
	if (CCurlingGame_Manager::GetInstance()->Is_FinishGame())
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField05* CSubQuestNode_NoisySnowField05::Create()
{
	CSubQuestNode_NoisySnowField05* pInstance = new CSubQuestNode_NoisySnowField05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField05::Free()
{
	__super::Free();
}

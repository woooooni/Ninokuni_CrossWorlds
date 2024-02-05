#include "stdafx.h"
#include "MainQuestNode_Glanix07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_Glanix07::CMainQuestNode_Glanix07()
{
}

HRESULT CMainQuestNode_Glanix07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("다시 에스타나비아로");
	m_strQuestContent = TEXT("왔던 길을 돌아가 에스타나비아로 돌아가기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("성으로");
	m_strNextQuestContent = TEXT("성으로 돌아가기");

	return S_OK;
}

void CMainQuestNode_Glanix07::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
}

CBTNode::NODE_STATE CMainQuestNode_Glanix07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strNextQuestTag;
		QuestDesc.strTitle = m_strNextQuestName;
		QuestDesc.strContents = m_strNextQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix07::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Glanix07* CMainQuestNode_Glanix07::Create()
{
	CMainQuestNode_Glanix07* pInstance = new CMainQuestNode_Glanix07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix07::Free()
{
	__super::Free();
}

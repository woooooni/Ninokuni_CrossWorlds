#include "stdafx.h"
#include "MainQuestNode_Glanix08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_Glanix08::CMainQuestNode_Glanix08()
{
}

HRESULT CMainQuestNode_Glanix08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("성으로");
	m_strQuestContent = TEXT("성으로 가기");

	return S_OK;
}

void CMainQuestNode_Glanix08::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_Glanix08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_KINGDOMHALL)
	{
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix08::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Glanix08* CMainQuestNode_Glanix08::Create()
{
	CMainQuestNode_Glanix08* pInstance = new CMainQuestNode_Glanix08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix08::Free()
{
	__super::Free();
}

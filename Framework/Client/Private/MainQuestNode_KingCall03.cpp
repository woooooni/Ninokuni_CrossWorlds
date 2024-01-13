#include "stdafx.h"
#include "MainQuestNode_KingCall03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_KingCall03::CMainQuestNode_KingCall03()
{
}

HRESULT CMainQuestNode_KingCall03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("성으로");
	m_strQuestContent = TEXT("성으로 가기");

	return S_OK;
}

void CMainQuestNode_KingCall03::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_KingCall03::Tick(const _float& fTimeDelta)
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

void CMainQuestNode_KingCall03::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_KingCall03* CMainQuestNode_KingCall03::Create()
{
	CMainQuestNode_KingCall03* pInstance = new CMainQuestNode_KingCall03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall03::Free()
{
	__super::Free();
}

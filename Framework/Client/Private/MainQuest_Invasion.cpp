#include "stdafx.h"
#include "MainQuest_Invasion.h"

#include "BTNode_Select.h"

#include "MainQuestNode_Invasion01.h"
#include "MainQuestNode_Invasion02.h"
#include "MainQuestNode_Invasion03.h"
#include "MainQuestNode_Invasion04.h"


CMainQuest_Invasion::CMainQuest_Invasion()
{
}

HRESULT CMainQuest_Invasion::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_Invasion01* pMainQuestInvasion01 = CMainQuestNode_Invasion01::Create();
	CMainQuestNode_Invasion02* pMainQuestInvasion02 = CMainQuestNode_Invasion02::Create();
	CMainQuestNode_Invasion03* pMainQuestInvasion03 = CMainQuestNode_Invasion03::Create();
	CMainQuestNode_Invasion04* pMainQuestInvasion04 = CMainQuestNode_Invasion04::Create();

	m_pRootNode->Add_ChildNode(pMainQuestInvasion01);
	m_pRootNode->Add_ChildNode(pMainQuestInvasion02);
	m_pRootNode->Add_ChildNode(pMainQuestInvasion03);
	m_pRootNode->Add_ChildNode(pMainQuestInvasion04);

	return S_OK;
}

_bool CMainQuest_Invasion::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_Invasion::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_Invasion* CMainQuest_Invasion::Create()
{
	CMainQuest_Invasion* pInstance = new CMainQuest_Invasion();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_Invasion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_Invasion::Free()
{
	__super::Free();
}


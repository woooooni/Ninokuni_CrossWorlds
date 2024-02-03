#include "stdafx.h"
#include "MainQuest_Ending.h"

#include "BTNode_Select.h"

#include "MainQuestNode_Ending01.h"
#include "MainQuestNode_Ending02.h"
#include "MainQuestNode_Ending03.h"
#include "MainQuestNode_Ending04.h"
#include "MainQuestNode_Ending05.h"

CMainQuest_Ending::CMainQuest_Ending()
{
}

HRESULT CMainQuest_Ending::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_Ending01* pMainQuestEnding01 = CMainQuestNode_Ending01::Create();
	CMainQuestNode_Ending02* pMainQuestEnding02 = CMainQuestNode_Ending02::Create();
	CMainQuestNode_Ending03* pMainQuestEnding03 = CMainQuestNode_Ending03::Create();
	CMainQuestNode_Ending04* pMainQuestEnding04 = CMainQuestNode_Ending04::Create();
	CMainQuestNode_Ending05* pMainQuestEnding05 = CMainQuestNode_Ending05::Create();

	m_pRootNode->Add_ChildNode(pMainQuestEnding01);
	m_pRootNode->Add_ChildNode(pMainQuestEnding02);
	m_pRootNode->Add_ChildNode(pMainQuestEnding03);
	m_pRootNode->Add_ChildNode(pMainQuestEnding04);
	m_pRootNode->Add_ChildNode(pMainQuestEnding05);

	return S_OK;
}

_bool CMainQuest_Ending::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_Ending::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_Ending* CMainQuest_Ending::Create()
{
	CMainQuest_Ending* pInstance = new CMainQuest_Ending();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_Ending::Free()
{
	__super::Free();
}


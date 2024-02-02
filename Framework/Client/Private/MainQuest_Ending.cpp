#include "stdafx.h"
#include "MainQuest_Ending.h"

#include "BTNode_Select.h"

#include "MainQuestNode_Ending01.h"

CMainQuest_Ending::CMainQuest_Ending()
{
}

HRESULT CMainQuest_Ending::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_Ending01* pMainQuestEnding01 = CMainQuestNode_Ending01::Create();

	m_pRootNode->Add_ChildNode(pMainQuestEnding01);

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


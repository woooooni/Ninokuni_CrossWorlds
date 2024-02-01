#include "stdafx.h"
#include "MainQuest_WitchForest.h"

#include "BTNode_Select.h"

#include "MainQuestNode_WitchForest01.h"
#include "MainQuestNode_WitchForest02.h"
#include "MainQuestNode_WitchForest03.h"
#include "MainQuestNode_WitchForest04.h"
#include "MainQuestNode_WitchForest04_2.h"
#include "MainQuestNode_WitchForest05.h"
#include "MainQuestNode_WitchForest05_2.h"
#include "MainQuestNode_WitchForest06.h"
#include "MainQuestNode_WitchForest07.h"
#include "MainQuestNode_WitchForest08.h"

CMainQuest_WitchForest::CMainQuest_WitchForest()
{
}

HRESULT CMainQuest_WitchForest::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_WitchForest01* pMainQuestWitchForest01 = CMainQuestNode_WitchForest01::Create();
	CMainQuestNode_WitchForest02* pMainQuestWitchForest02 = CMainQuestNode_WitchForest02::Create();
	CMainQuestNode_WitchForest03* pMainQuestWitchForest03 = CMainQuestNode_WitchForest03::Create();
	CMainQuestNode_WitchForest04* pMainQuestWitchForest04 = CMainQuestNode_WitchForest04::Create();
	CMainQuestNode_WitchForest04_2* pMainQuestWitchForest04_2 = CMainQuestNode_WitchForest04_2::Create();
	CMainQuestNode_WitchForest05* pMainQuestWitchForest05 = CMainQuestNode_WitchForest05::Create();
	CMainQuestNode_WitchForest05_2* pMainQuestWitchForest05_2 = CMainQuestNode_WitchForest05_2::Create();
	CMainQuestNode_WitchForest06* pMainQuestWitchForest06 = CMainQuestNode_WitchForest06::Create();
	CMainQuestNode_WitchForest07* pMainQuestWitchForest07 = CMainQuestNode_WitchForest07::Create();
	CMainQuestNode_WitchForest08* pMainQuestWitchForest08 = CMainQuestNode_WitchForest08::Create();

	m_pRootNode->Add_ChildNode(pMainQuestWitchForest01);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest02);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest03);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest04);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest04_2);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest05);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest05_2);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest06);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest07);
	m_pRootNode->Add_ChildNode(pMainQuestWitchForest08);

	return S_OK;
}

_bool CMainQuest_WitchForest::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_WitchForest::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_WitchForest* CMainQuest_WitchForest::Create()
{
	CMainQuest_WitchForest* pInstance = new CMainQuest_WitchForest();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_WitchForest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_WitchForest::Free()
{
	__super::Free();
}


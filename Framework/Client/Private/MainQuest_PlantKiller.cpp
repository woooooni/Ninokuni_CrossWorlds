#include "stdafx.h"
#include "MainQuest_PlantKiller.h"

#include "BTNode_Select.h"

#include "MainQuestNode_PlantKiller01.h"
#include "MainQuestNode_PlantKiller02.h"
#include "MainQuestNode_PlantKiller03.h"
#include "MainQuestNode_PlantKiller04.h"
#include "MainQuestNode_PlantKiller05.h"
#include "MainQuestNode_PlantKiller06.h"
#include "MainQuestNode_PlantKiller07.h"
#include "MainQuestNode_PlantKiller08.h"
#include "MainQuestNode_PlantKiller09.h"
#include "MainQuestNode_PlantKiller10.h"
#include "MainQuestNode_PlantKiller11.h"

CMainQuest_PlantKiller::CMainQuest_PlantKiller()
{
}

HRESULT CMainQuest_PlantKiller::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_PlantKiller01* pMainQuestPlantKiller01 = CMainQuestNode_PlantKiller01::Create();
	CMainQuestNode_PlantKiller02* pMainQuestPlantKiller02 = CMainQuestNode_PlantKiller02::Create();
	CMainQuestNode_PlantKiller03* pMainQuestPlantKiller03 = CMainQuestNode_PlantKiller03::Create();
	CMainQuestNode_PlantKiller04* pMainQuestPlantKiller04 = CMainQuestNode_PlantKiller04::Create();
	CMainQuestNode_PlantKiller05* pMainQuestPlantKiller05 = CMainQuestNode_PlantKiller05::Create();
	CMainQuestNode_PlantKiller06* pMainQuestPlantKiller06 = CMainQuestNode_PlantKiller06::Create();
	CMainQuestNode_PlantKiller07* pMainQuestPlantKiller07 = CMainQuestNode_PlantKiller07::Create();
	CMainQuestNode_PlantKiller08* pMainQuestPlantKiller08 = CMainQuestNode_PlantKiller08::Create();
	CMainQuestNode_PlantKiller09* pMainQuestPlantKiller09 = CMainQuestNode_PlantKiller09::Create();
	CMainQuestNode_PlantKiller10* pMainQuestPlantKiller10 = CMainQuestNode_PlantKiller10::Create();
	CMainQuestNode_PlantKiller11* pMainQuestPlantKiller11 = CMainQuestNode_PlantKiller11::Create();

	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller01);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller02);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller03);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller04);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller05);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller06);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller07);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller08);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller09);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller10);
	m_pRootNode->Add_ChildNode(pMainQuestPlantKiller11);

	return S_OK;
}

_bool CMainQuest_PlantKiller::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_PlantKiller::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_PlantKiller* CMainQuest_PlantKiller::Create()
{
	CMainQuest_PlantKiller* pInstance = new CMainQuest_PlantKiller();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_PlantKiller");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_PlantKiller::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "MainQuest_Glanix.h"

#include "BTNode_Select.h"

#include "MainQuestNode_Glanix01.h"
#include "MainQuestNode_Glanix02.h"
#include "MainQuestNode_Glanix03.h"
#include "MainQuestNode_Glanix04.h"
#include "MainQuestNode_Glanix05.h"
#include "MainQuestNode_Glanix06.h"
#include "MainQuestNode_Glanix07.h"
#include "MainQuestNode_Glanix08.h"
#include "MainQuestNode_Glanix09.h"
#include "MainQuestNode_Glanix10.h"

CMainQuest_Glanix::CMainQuest_Glanix()
{
}

HRESULT CMainQuest_Glanix::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_Glanix01* pMainQuestGlanix01 = CMainQuestNode_Glanix01::Create();
	CMainQuestNode_Glanix02* pMainQuestGlanix02 = CMainQuestNode_Glanix02::Create();
	CMainQuestNode_Glanix03* pMainQuestGlanix03 = CMainQuestNode_Glanix03::Create();
	CMainQuestNode_Glanix04* pMainQuestGlanix04 = CMainQuestNode_Glanix04::Create();
	CMainQuestNode_Glanix05* pMainQuestGlanix05 = CMainQuestNode_Glanix05::Create();
	CMainQuestNode_Glanix06* pMainQuestGlanix06 = CMainQuestNode_Glanix06::Create();
	CMainQuestNode_Glanix07* pMainQuestGlanix07 = CMainQuestNode_Glanix07::Create();
	CMainQuestNode_Glanix08* pMainQuestGlanix08 = CMainQuestNode_Glanix08::Create();
	CMainQuestNode_Glanix09* pMainQuestGlanix09 = CMainQuestNode_Glanix09::Create();
	CMainQuestNode_Glanix10* pMainQuestGlanix10 = CMainQuestNode_Glanix10::Create();

	m_pRootNode->Add_ChildNode(pMainQuestGlanix01);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix02);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix03);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix04);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix05);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix06);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix07);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix08);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix09);
	m_pRootNode->Add_ChildNode(pMainQuestGlanix10);

	return S_OK;
}

_bool CMainQuest_Glanix::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_Glanix::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_Glanix* CMainQuest_Glanix::Create()
{
	CMainQuest_Glanix* pInstance = new CMainQuest_Glanix();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_Glanix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_Glanix::Free()
{
	__super::Free();
}


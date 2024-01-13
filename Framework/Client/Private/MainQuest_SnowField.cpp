#include "stdafx.h"
#include "MainQuest_SnowField.h"

#include "BTNode_Select.h"

#include "MainQuestNode_SnowField01.h"
#include "MainQuestNode_SnowField02.h"
#include "MainQuestNode_SnowField02_2.h"
#include "MainQuestNode_SnowField03.h"
#include "MainQuestNode_SnowField04.h"
#include "MainQuestNode_SnowField05.h"
#include "MainQuestNode_SnowField06.h"
#include "MainQuestNode_SnowField07.h"

CMainQuest_SnowField::CMainQuest_SnowField()
{
}

HRESULT CMainQuest_SnowField::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_SnowField01* pMainQuestSnowField01 = CMainQuestNode_SnowField01::Create();
	CMainQuestNode_SnowField02* pMainQuestSnowField02 = CMainQuestNode_SnowField02::Create();
	CMainQuestNode_SnowField02_2* pMainQuestSnowField02_2 = CMainQuestNode_SnowField02_2::Create();
	CMainQuestNode_SnowField03* pMainQuestSnowField03 = CMainQuestNode_SnowField03::Create();
	CMainQuestNode_SnowField04* pMainQuestSnowField04 = CMainQuestNode_SnowField04::Create();
	CMainQuestNode_SnowField05* pMainQuestSnowField05 = CMainQuestNode_SnowField05::Create();
	CMainQuestNode_SnowField06* pMainQuestSnowField06 = CMainQuestNode_SnowField06::Create();
	CMainQuestNode_SnowField07* pMainQuestSnowField07 = CMainQuestNode_SnowField07::Create();

	m_pRootNode->Add_ChildNode(pMainQuestSnowField01);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField02);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField02_2);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField03);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField04);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField05);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField06);
	m_pRootNode->Add_ChildNode(pMainQuestSnowField07);

	return S_OK;
}

_bool CMainQuest_SnowField::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_SnowField::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_SnowField* CMainQuest_SnowField::Create()
{
	CMainQuest_SnowField* pInstance = new CMainQuest_SnowField();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_SnowField");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_SnowField::Free()
{
	__super::Free();
}


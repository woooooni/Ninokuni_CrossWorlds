#include "stdafx.h"
#include "MainQuest_FinalBattle.h"

#include "BTNode_Select.h"

#include "MainQuestNode_FinalBattle01.h"
#include "MainQuestNode_FinalBattle02.h"
#include "MainQuestNode_FinalBattle03.h"
#include "MainQuestNode_FinalBattle04.h"
#include "MainQuestNode_FinalBattle05.h"
#include "MainQuestNode_FinalBattle06.h"
#include "MainQuestNode_FinalBattle07.h"

CMainQuest_FinalBattle::CMainQuest_FinalBattle()
{
}

HRESULT CMainQuest_FinalBattle::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_FinalBattle01* pMainQuestFinalBattle01 = CMainQuestNode_FinalBattle01::Create();
	CMainQuestNode_FinalBattle02* pMainQuestFinalBattle02 = CMainQuestNode_FinalBattle02::Create();
	CMainQuestNode_FinalBattle03* pMainQuestFinalBattle03 = CMainQuestNode_FinalBattle03::Create();
	CMainQuestNode_FinalBattle04* pMainQuestFinalBattle04 = CMainQuestNode_FinalBattle04::Create();
	CMainQuestNode_FinalBattle05* pMainQuestFinalBattle05 = CMainQuestNode_FinalBattle05::Create();
	CMainQuestNode_FinalBattle06* pMainQuestFinalBattle06 = CMainQuestNode_FinalBattle06::Create();
	CMainQuestNode_FinalBattle07* pMainQuestFinalBattle07 = CMainQuestNode_FinalBattle07::Create();

	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle01);
	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle02);
	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle03);
	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle04);
	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle05);
	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle06);
	m_pRootNode->Add_ChildNode(pMainQuestFinalBattle07);

	return S_OK;
}

_bool CMainQuest_FinalBattle::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_FinalBattle::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_FinalBattle* CMainQuest_FinalBattle::Create()
{
	CMainQuest_FinalBattle* pInstance = new CMainQuest_FinalBattle();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_FinalBattle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_FinalBattle::Free()
{
	__super::Free();
}


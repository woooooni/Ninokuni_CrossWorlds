#include "stdafx.h"
#include "MainQuest_KingCall.h"

#include "BTNode_Select.h"

#include "MainQuestNode_KingCall01.h"
#include "MainQuestNode_KingCall02.h"
#include "MainQuestNode_KingCall03.h"
#include "MainQuestNode_KingCall04.h"
#include "MainQuestNode_KingCall05.h"
#include "MainQuestNode_KingCall06.h"

CMainQuest_KingCall::CMainQuest_KingCall()
{
}

HRESULT CMainQuest_KingCall::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	CMainQuestNode_KingCall01* pMainQuestKingCall01 = CMainQuestNode_KingCall01::Create();
	CMainQuestNode_KingCall02* pMainQuestKingCall02 = CMainQuestNode_KingCall02::Create();
	CMainQuestNode_KingCall03* pMainQuestKingCall03 = CMainQuestNode_KingCall03::Create();
	CMainQuestNode_KingCall04* pMainQuestKingCall04 = CMainQuestNode_KingCall04::Create();
	CMainQuestNode_KingCall05* pMainQuestKingCall05 = CMainQuestNode_KingCall05::Create();
	CMainQuestNode_KingCall06* pMainQuestKingCall06 = CMainQuestNode_KingCall06::Create();

	m_pRootNode->Add_ChildNode(pMainQuestKingCall01);
	m_pRootNode->Add_ChildNode(pMainQuestKingCall02);
	m_pRootNode->Add_ChildNode(pMainQuestKingCall03);
	m_pRootNode->Add_ChildNode(pMainQuestKingCall04);
	m_pRootNode->Add_ChildNode(pMainQuestKingCall05);
	m_pRootNode->Add_ChildNode(pMainQuestKingCall06);

	return S_OK;
}

_bool CMainQuest_KingCall::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_KingCall::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_KingCall* CMainQuest_KingCall::Create()
{
	CMainQuest_KingCall* pInstance = new CMainQuest_KingCall();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_KingCall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_KingCall::Free()
{
	__super::Free();
}


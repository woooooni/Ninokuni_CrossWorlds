#include "stdafx.h"
#include "SubQuest_Wanted.h"

#include "BTNode_Select.h"

#include "SubQuestNode_Wanted01.h"
#include "SubQuestNode_Wanted02.h"
#include "SubQuestNode_Wanted03.h"
#include "SubQuestNode_Wanted04.h"
#include "SubQuestNode_Wanted05.h"
#include "SubQuestNode_Wanted06.h"
#include "SubQuestNode_Wanted07.h"
#include "SubQuestNode_Wanted08.h"

CSubQuest_Wanted::CSubQuest_Wanted()
{
}

HRESULT CSubQuest_Wanted::Initialize()
{
	__super::Initialize();

	m_bIsRunningQuest = false;

	m_pRootNode = CBTNode_Select::Create();

	CSubQuestNode_Wanted01* pSubQuest_Wanted01 = CSubQuestNode_Wanted01::Create();
	CSubQuestNode_Wanted02* pSubQuest_Wanted02 = CSubQuestNode_Wanted02::Create();
	CSubQuestNode_Wanted03* pSubQuest_Wanted03 = CSubQuestNode_Wanted03::Create();
	CSubQuestNode_Wanted04* pSubQuest_Wanted04 = CSubQuestNode_Wanted04::Create();
	CSubQuestNode_Wanted05* pSubQuest_Wanted05 = CSubQuestNode_Wanted05::Create();
	CSubQuestNode_Wanted06* pSubQuest_Wanted06 = CSubQuestNode_Wanted06::Create();
	CSubQuestNode_Wanted07* pSubQuest_Wanted07 = CSubQuestNode_Wanted07::Create();
	CSubQuestNode_Wanted08* pSubQuest_Wanted08 = CSubQuestNode_Wanted08::Create();

	m_pRootNode->Add_ChildNode(pSubQuest_Wanted01);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted02);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted03);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted04);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted05);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted06);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted07);
	m_pRootNode->Add_ChildNode(pSubQuest_Wanted08);

	return S_OK;
}

_bool CSubQuest_Wanted::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CSubQuest_Wanted::LateTick(const _float& fTimeDelta)
{
}

CSubQuest_Wanted* CSubQuest_Wanted::Create()
{
	CSubQuest_Wanted* pInstance = new CSubQuest_Wanted();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuest_Wanted");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuest_Wanted::Free()
{
	__super::Free();
}


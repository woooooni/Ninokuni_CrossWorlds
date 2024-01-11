#include "stdafx.h"
#include "SubQuest_Windmill.h"

#include "BTNode_Select.h"

#include "SubQuestNode_Windmill01.h"
#include "SubQuestNode_Windmill02.h"
#include "SubQuestNode_Windmill03.h"
#include "SubQuestNode_Windmill04.h"
#include "SubQuestNode_Windmill05.h"
#include "SubQuestNode_Windmill06.h"
#include "SubQuestNode_Windmill07.h"
#include "SubQuestNode_Windmill08.h"
#include "SubQuestNode_Windmill09.h"
#include "SubQuestNode_Windmill10.h"
#include "SubQuestNode_Windmill11.h"
#include "SubQuestNode_Windmill12.h"

CSubQuest_Windmill::CSubQuest_Windmill()
{
}

HRESULT CSubQuest_Windmill::Initialize()
{
	__super::Initialize();

	/* 그렴 여기서도 노드로 하는게 아니라 그냥 배열로 가지고 bool로 처리를 해야한다.*/

	m_pRootNode = CBTNode_Select::Create();

	CSubQuestNode_Windmill01* pSubQuest_Windmill01 = CSubQuestNode_Windmill01::Create();
	CSubQuestNode_Windmill02* pSubQuest_Windmill02 = CSubQuestNode_Windmill02::Create();
	CSubQuestNode_Windmill03* pSubQuest_Windmill03 = CSubQuestNode_Windmill03::Create();
	CSubQuestNode_Windmill04* pSubQuest_Windmill04 = CSubQuestNode_Windmill04::Create();
	CSubQuestNode_Windmill05* pSubQuest_Windmill05 = CSubQuestNode_Windmill05::Create();
	CSubQuestNode_Windmill06* pSubQuest_Windmill06 = CSubQuestNode_Windmill06::Create();
	CSubQuestNode_Windmill07* pSubQuest_Windmill07 = CSubQuestNode_Windmill07::Create();
	CSubQuestNode_Windmill08* pSubQuest_Windmill08 = CSubQuestNode_Windmill08::Create();
	CSubQuestNode_Windmill09* pSubQuest_Windmill09 = CSubQuestNode_Windmill09::Create();
	CSubQuestNode_Windmill10* pSubQuest_Windmill10 = CSubQuestNode_Windmill10::Create();
	CSubQuestNode_Windmill11* pSubQuest_Windmill11 = CSubQuestNode_Windmill11::Create();
	CSubQuestNode_Windmill12* pSubQuest_Windmill12 = CSubQuestNode_Windmill12::Create();

	m_pRootNode->Add_ChildNode(pSubQuest_Windmill01);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill02);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill03);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill04);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill05);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill06);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill07);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill08);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill09);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill10);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill11);
	m_pRootNode->Add_ChildNode(pSubQuest_Windmill12);

	return S_OK;
}

_bool CSubQuest_Windmill::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CSubQuest_Windmill::LateTick(const _float& fTimeDelta)
{
}

CSubQuest_Windmill* CSubQuest_Windmill::Create()
{
	CSubQuest_Windmill* pInstance = new CSubQuest_Windmill();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuest_Windmill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuest_Windmill::Free()
{
	__super::Free();
}


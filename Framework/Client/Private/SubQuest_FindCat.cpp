#include "stdafx.h"
#include "SubQuest_FindCat.h"

#include "BTNode_Select.h"

#include "SubQuestNode_FindCat01.h"
#include "SubQuestNode_FindCat02.h"
#include "SubQuestNode_FindCat03.h"
#include "SubQuestNode_FindCat04.h"
#include "SubQuestNode_FindCat05.h"
#include "SubQuestNode_FindCat06.h"

CSubQuest_FindCat::CSubQuest_FindCat()
{
}

HRESULT CSubQuest_FindCat::Initialize()
{
	__super::Initialize();

	m_bIsRunningQuest = false;

	m_pRootNode = CBTNode_Select::Create();

	CSubQuestNode_FindCat01* pSubQuest_FindCat01 = CSubQuestNode_FindCat01::Create();
	CSubQuestNode_FindCat02* pSubQuest_FindCat02 = CSubQuestNode_FindCat02::Create();
	CSubQuestNode_FindCat03* pSubQuest_FindCat03 = CSubQuestNode_FindCat03::Create();
	CSubQuestNode_FindCat04* pSubQuest_FindCat04 = CSubQuestNode_FindCat04::Create();
	CSubQuestNode_FindCat05* pSubQuest_FindCat05 = CSubQuestNode_FindCat05::Create();
	CSubQuestNode_FindCat06* pSubQuest_FindCat06 = CSubQuestNode_FindCat06::Create();

	m_pRootNode->Add_ChildNode(pSubQuest_FindCat01);
	m_pRootNode->Add_ChildNode(pSubQuest_FindCat02);
	m_pRootNode->Add_ChildNode(pSubQuest_FindCat03);
	m_pRootNode->Add_ChildNode(pSubQuest_FindCat04);
	m_pRootNode->Add_ChildNode(pSubQuest_FindCat05);
	m_pRootNode->Add_ChildNode(pSubQuest_FindCat06);

	return S_OK;
}

_bool CSubQuest_FindCat::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
	{
		m_bIsRunningQuest = false;
		m_bIsClearQuest = true;
		return true;
	}

	return false;
}

void CSubQuest_FindCat::LateTick(const _float& fTimeDelta)
{
}

CSubQuest_FindCat* CSubQuest_FindCat::Create()
{
	CSubQuest_FindCat* pInstance = new CSubQuest_FindCat();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuest_FindCat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuest_FindCat::Free()
{
	__super::Free();
}


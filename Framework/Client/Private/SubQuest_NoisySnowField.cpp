#include "stdafx.h"
#include "SubQuest_NoisySnowField.h"

#include "BTNode_Select.h"

#include "SubQuestNode_NoisySnowField01.h"
#include "SubQuestNode_NoisySnowField02.h"
#include "SubQuestNode_NoisySnowField03.h"
#include "SubQuestNode_NoisySnowField04.h"
#include "SubQuestNode_NoisySnowField04_2.h"
#include "SubQuestNode_NoisySnowField05.h"
#include "SubQuestNode_NoisySnowField06.h"
#include "SubQuestNode_NoisySnowField07.h"
#include "SubQuestNode_NoisySnowField08.h"

CSubQuest_NoisySnowField::CSubQuest_NoisySnowField()
{
}

HRESULT CSubQuest_NoisySnowField::Initialize()
{
	__super::Initialize();

	m_bIsRunningQuest = false;

	m_pRootNode = CBTNode_Select::Create();

	CSubQuestNode_NoisySnowField01* pSubQuest_NoisySnowField01 = CSubQuestNode_NoisySnowField01::Create();
	CSubQuestNode_NoisySnowField02* pSubQuest_NoisySnowField02 = CSubQuestNode_NoisySnowField02::Create();
	CSubQuestNode_NoisySnowField03* pSubQuest_NoisySnowField03 = CSubQuestNode_NoisySnowField03::Create();
	CSubQuestNode_NoisySnowField04* pSubQuest_NoisySnowField04 = CSubQuestNode_NoisySnowField04::Create();
	CSubQuestNode_NoisySnowField04_2* pSubQuest_NoisySnowField04_2 = CSubQuestNode_NoisySnowField04_2::Create();
	CSubQuestNode_NoisySnowField05* pSubQuest_NoisySnowField05 = CSubQuestNode_NoisySnowField05::Create();
	CSubQuestNode_NoisySnowField06* pSubQuest_NoisySnowField06 = CSubQuestNode_NoisySnowField06::Create();
	CSubQuestNode_NoisySnowField07* pSubQuest_NoisySnowField07 = CSubQuestNode_NoisySnowField07::Create();
	CSubQuestNode_NoisySnowField08* pSubQuest_NoisySnowField08 = CSubQuestNode_NoisySnowField08::Create();

	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField01);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField02);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField03);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField04);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField04_2);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField05);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField06);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField07);
	m_pRootNode->Add_ChildNode(pSubQuest_NoisySnowField08);

	return S_OK;
}

_bool CSubQuest_NoisySnowField::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CSubQuest_NoisySnowField::LateTick(const _float& fTimeDelta)
{
}

CSubQuest_NoisySnowField* CSubQuest_NoisySnowField::Create()
{
	CSubQuest_NoisySnowField* pInstance = new CSubQuest_NoisySnowField();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuest_NoisySnowField");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuest_NoisySnowField::Free()
{
	__super::Free();
}


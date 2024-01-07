#include "stdafx.h"
#include "MainQuest01.h"

#include "BTNode_Select.h"

#include "MainQuestNode01_01.h"

CMainQuest01::CMainQuest01()
{
}

HRESULT CMainQuest01::Initialize()
{
	__super::Initialize();

	/* 그렴 여기서도 노드로 하는게 아니라 그냥 배열로 가지고 bool로 처리를 해야한다.*/

	m_pRootNode = CBTNode_Select::Create();
	
	CMainQuestNode01_01* m_pMainQuest01_01 = CMainQuestNode01_01::Create();

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_iQuestIndex += 1;

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_iQuestIndex += 1;

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_iQuestIndex += 1;

	return S_OK;
}

_bool CMainQuest01::Tick(const _float& fTimeDelta)
{
	// 이렇게 하면 안되겠는데? ㅁㅊ
	m_pRootNode->Tick(fTimeDelta);

	return false;
}

void CMainQuest01::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}


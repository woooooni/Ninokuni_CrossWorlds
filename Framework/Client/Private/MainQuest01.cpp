#include "stdafx.h"
#include "MainQuest01.h"

#include "BTNode_Select.h"

#include "MainQuestNode01_01.h"
#include "MainQuestNode01_02.h"
#include "MainQuestNode01_03.h"

CMainQuest01::CMainQuest01()
{
}

HRESULT CMainQuest01::Initialize()
{
	__super::Initialize();

	/* 그렴 여기서도 노드로 하는게 아니라 그냥 배열로 가지고 bool로 처리를 해야한다.*/

	m_pRootNode = CBTNode_Select::Create();
	
	CMainQuestNode01_01* m_pMainQuest01_01 = CMainQuestNode01_01::Create();
	CMainQuestNode01_02* m_pMainQuest01_02 = CMainQuestNode01_02::Create();
	CMainQuestNode01_03* m_pMainQuest01_03 = CMainQuestNode01_03::Create();

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_pRootNode->Add_ChildNode(m_pMainQuest01_02);
	m_pRootNode->Add_ChildNode(m_pMainQuest01_03);

	return S_OK;
}

_bool CMainQuest01::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest01::LateTick(const _float& fTimeDelta)
{
}

CMainQuest01* CMainQuest01::Create()
{
	CMainQuest01* pInstance = new CMainQuest01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest01::Free()
{
	__super::Free();
}


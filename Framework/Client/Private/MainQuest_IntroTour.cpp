#include "stdafx.h"
#include "MainQuest_IntroTour.h"

#include "BTNode_Select.h"

#include "MainQuestNode_IntroTour01.h"
#include "MainQuestNode_IntroTour02.h"
#include "MainQuestNode_IntroTour03.h"
#include "MainQuestNode_IntroTour04.h"


CMainQuest_IntroTour::CMainQuest_IntroTour()
{
}

HRESULT CMainQuest_IntroTour::Initialize()
{
	__super::Initialize();

	/* 그렴 여기서도 노드로 하는게 아니라 그냥 배열로 가지고 bool로 처리를 해야한다.*/

	m_pRootNode = CBTNode_Select::Create();
	
	CMainQuestNode_IntroTour01* pMainQuestIntroTour01 = CMainQuestNode_IntroTour01::Create();
	CMainQuestNode_IntroTour02* pMainQuestIntroTour02 = CMainQuestNode_IntroTour02::Create();
	CMainQuestNode_IntroTour03* pMainQuestIntroTour03 = CMainQuestNode_IntroTour03::Create();
	CMainQuestNode_IntroTour04* pMainQuestIntroTour04 = CMainQuestNode_IntroTour04::Create();

	m_pRootNode->Add_ChildNode(pMainQuestIntroTour01);
	m_pRootNode->Add_ChildNode(pMainQuestIntroTour02);
	m_pRootNode->Add_ChildNode(pMainQuestIntroTour03);
	m_pRootNode->Add_ChildNode(pMainQuestIntroTour04);

	return S_OK;
}

_bool CMainQuest_IntroTour::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_IntroTour::LateTick(const _float& fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

CMainQuest_IntroTour* CMainQuest_IntroTour::Create()
{
	CMainQuest_IntroTour* pInstance = new CMainQuest_IntroTour();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_IntroTour");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_IntroTour::Free()
{
	__super::Free();
}


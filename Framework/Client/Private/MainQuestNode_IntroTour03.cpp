#include "stdafx.h"
#include "MainQuestNode_IntroTour03.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "GameInstance.h"
#include "Utils.h"
#include "UI_PopupQuest.h"

CMainQuestNode_IntroTour03::CMainQuestNode_IntroTour03()
{
}

HRESULT CMainQuestNode_IntroTour03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("에스타바니아 둘러보기");

	return S_OK;
}

void CMainQuestNode_IntroTour03::Start()
{
	m_strQuestContent = L"서브 퀘스트 " + to_wstring(CQuest_Manager::GetInstance()->Get_QuestClearStack());
	m_strQuestContent = m_strQuestContent + L" / 3 완료";

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
}

CBTNode::NODE_STATE CMainQuestNode_IntroTour03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_iPrevClearCount != CQuest_Manager::GetInstance()->Get_QuestClearStack())
	{
		m_strQuestContent = L"서브 퀘스트 " + to_wstring(CQuest_Manager::GetInstance()->Get_QuestClearStack());
		m_strQuestContent = m_strQuestContent + L" / 3 완료";

		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

		m_iPrevClearCount += CQuest_Manager::GetInstance()->Get_QuestClearStack();
	}

	if (CQuest_Manager::GetInstance()->Get_QuestClearStack() >= 3)
	{
		m_bIsClear = true;
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_IntroTour03::LateTick(const _float& fTimeDelta)
{
	if (m_pQuestDestSpot != nullptr)
		m_pQuestDestSpot->LateTick(fTimeDelta);
}

CMainQuestNode_IntroTour03* CMainQuestNode_IntroTour03::Create()
{
	CMainQuestNode_IntroTour03* pInstance = new CMainQuestNode_IntroTour03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_IntroTour03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_IntroTour03::Free()
{
	__super::Free();
	Safe_Release(m_pQuestDestSpot);
}

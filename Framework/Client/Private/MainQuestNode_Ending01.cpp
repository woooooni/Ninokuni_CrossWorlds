#include "stdafx.h"
#include "MainQuestNode_Ending01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"
#include "UI_PopupQuest.h"
#include "Effect_Manager.h"

CMainQuestNode_Ending01::CMainQuestNode_Ending01()
{
}

HRESULT CMainQuestNode_Ending01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("이제 우리는 부자다");
	m_strQuestContent = TEXT("에스타나비아로 돌아가자");

	return S_OK;
}

void CMainQuestNode_Ending01::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::GetInstance()->QUESTEVENT_ENDING);

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_Ending01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		// Effect
		_matrix WorldMatrix = XMMatrixIdentity();
		WorldMatrix.r[3] = XMVectorSet(0.f, 30.f, 60.f, 1.f);
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_FireCracker"), WorldMatrix);

		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Ending01::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Ending01* CMainQuestNode_Ending01::Create()
{
	CMainQuestNode_Ending01* pInstance = new CMainQuestNode_Ending01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Ending01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Ending01::Free()
{
	__super::Free();
}

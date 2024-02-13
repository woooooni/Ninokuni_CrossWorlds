#include "stdafx.h"
#include "MainQuestNode_Ending03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_Ending03::CMainQuestNode_Ending03()
{
}

HRESULT CMainQuestNode_Ending03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("이야기의 끝");
	m_strQuestContent = TEXT("길었던 이야기의 끝을 맺을 시간이다");

	return S_OK;
}

void CMainQuestNode_Ending03::Start()
{
//	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Ruslan"));
	if (m_pRuslan != nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pRuslan);

		// 임시로 monster에 
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		QuestDesc.bCreateSpot = true;
		QuestDesc.vDestPosition = vSpotPos;
		CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
	}
	else
	{
		MSG_BOX("MainQuest_Ending03 : MainQuestNode_Ending03, Ruslan Create Fail");
	}
}

CBTNode::NODE_STATE CMainQuestNode_Ending03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot->Get_IsCol())
			{
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

				m_bIsClear = true;
				m_pQuestDestSpot->Set_ReadyDelete(true);
				Safe_Release(m_pQuestDestSpot);
				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Ending03::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Ending03* CMainQuestNode_Ending03::Create()
{
	CMainQuestNode_Ending03* pInstance = new CMainQuestNode_Ending03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Ending03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Ending03::Free()
{
	__super::Free();
}

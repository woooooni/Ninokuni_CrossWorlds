#include "stdafx.h"
#include "MainQuestNode_WitchForest05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_WitchForest05::CMainQuestNode_WitchForest05()
{
}

HRESULT CMainQuestNode_WitchForest05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("작은 마녀의 마을");
	m_strQuestContent = TEXT("큰 집 앞의 마녀에게 단서를 찾아보자");

	return S_OK;
}

void CMainQuestNode_WitchForest05::Start()
{
	m_pCyan = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("Cyan"));
	Vec4 vSpotPos = Set_DestSpot(m_pCyan);

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

CBTNode::NODE_STATE CMainQuestNode_WitchForest05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
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
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest05::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_WitchForest05* CMainQuestNode_WitchForest05::Create()
{
	CMainQuestNode_WitchForest05* pInstance = new CMainQuestNode_WitchForest05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_WitchForest05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_WitchForest05::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "SubQuestNode_Windmill03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CSubQuestNode_Windmill03::CSubQuestNode_Windmill03()
{
}

HRESULT CSubQuestNode_Windmill03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("풍차 수리");
	m_strQuestContent = TEXT("비어드에게 가기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("풍차 수리");
	m_strNextQuestContent = TEXT("비어드에게 가기");


	return S_OK;
}

void CSubQuestNode_Windmill03::Start()
{
	m_pBeard = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("SwiftSolutionMaster"));
	Vec4 vSpotPos = Set_DestSpot(m_pBeard);

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

CBTNode::NODE_STATE CSubQuestNode_Windmill03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					CUI_PopupQuest::QUEST_INFO QuestDesc = {};
					QuestDesc.strType = m_strNextQuestTag;
					QuestDesc.strTitle = m_strNextQuestName;
					QuestDesc.strContents = m_strNextQuestContent;
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

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

void CSubQuestNode_Windmill03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill03* CSubQuestNode_Windmill03::Create()
{
	CSubQuestNode_Windmill03* pInstance = new CSubQuestNode_Windmill03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill03::Free()
{
	__super::Free();
}

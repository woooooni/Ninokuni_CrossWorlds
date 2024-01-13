#include "stdafx.h"
#include "SubQuestNode_FindCat04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_FindCat04::CSubQuestNode_FindCat04()
{
}

HRESULT CSubQuestNode_FindCat04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("고양이 찾기");
	m_strQuestContent = TEXT("클로이의 고양이 찾기");

	return S_OK;
}

void CSubQuestNode_FindCat04::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pChloe = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Chloe"));
	Vec4 vSpotPos = Set_DestSpot(m_pChloe);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_FindCat04::Tick(const _float& fTimeDelta)
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

void CSubQuestNode_FindCat04::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_FindCat04* CSubQuestNode_FindCat04::Create()
{
	CSubQuestNode_FindCat04* pInstance = new CSubQuestNode_FindCat04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_FindCat04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_FindCat04::Free()
{
	__super::Free();
}

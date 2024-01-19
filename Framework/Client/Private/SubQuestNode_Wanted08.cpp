#include "stdafx.h"
#include "SubQuestNode_Wanted08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted08::CSubQuestNode_Wanted08()
{
}

HRESULT CSubQuestNode_Wanted08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("툼바에게 돌아가기");
	m_strQuestContent = TEXT("툼바에게 돌아가자");

	return S_OK;
}

void CSubQuestNode_Wanted08::Start()
{
	m_pTumba = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("BlackSmithMaster"));
	Vec4 vSpotPos = Set_DestSpot(m_pTumba);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Wanted08::Tick(const _float& fTimeDelta)
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
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestTag, m_strQuestTag, m_strQuestName, m_strQuestContent);

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

void CSubQuestNode_Wanted08::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted08* CSubQuestNode_Wanted08::Create()
{
	CSubQuestNode_Wanted08* pInstance = new CSubQuestNode_Wanted08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted08::Free()
{
	__super::Free();
}

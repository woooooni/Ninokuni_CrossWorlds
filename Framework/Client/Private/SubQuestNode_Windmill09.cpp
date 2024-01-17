#include "stdafx.h"
#include "SubQuestNode_Windmill09.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Windmill09::CSubQuestNode_Windmill09()
{
}

HRESULT CSubQuestNode_Windmill09::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("풍차 수리");
	m_strQuestContent = TEXT("베르디에게 돌아가기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("풍차 수리");
	m_strNextQuestContent = TEXT("베르디에게 돌아가기");


	return S_OK;
}

void CSubQuestNode_Windmill09::Start()
{
	m_pVerde = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Verde"));
	Vec4 vSpotPos = Set_DestSpot(m_pVerde);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Windmill09::Tick(const _float& fTimeDelta)
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
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

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

void CSubQuestNode_Windmill09::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill09* CSubQuestNode_Windmill09::Create()
{
	CSubQuestNode_Windmill09* pInstance = new CSubQuestNode_Windmill09();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill09");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill09::Free()
{
	__super::Free();
}

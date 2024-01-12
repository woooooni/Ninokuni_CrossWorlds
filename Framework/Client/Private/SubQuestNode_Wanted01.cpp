#include "stdafx.h"
#include "SubQuestNode_Wanted01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted01::CSubQuestNode_Wanted01()
{
}

HRESULT CSubQuestNode_Wanted01::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CSubQuestNode_Wanted01::Start()
{
	m_pTumba = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("BlackSmithMaster"));
	Vec4 vSpotPos = Set_DestSpot(m_pTumba);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Wanted01::Tick(const _float& fTimeDelta)
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
					m_bIsClear = true;
					m_pQuestDestSpot->Set_Dead(true);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted01::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted01* CSubQuestNode_Wanted01::Create()
{
	CSubQuestNode_Wanted01* pInstance = new CSubQuestNode_Wanted01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted01::Free()
{
	__super::Free();
}

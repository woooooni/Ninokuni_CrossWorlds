#include "stdafx.h"
#include "SubQuestNode_NoisySnowField01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_NoisySnowField01::CSubQuestNode_NoisySnowField01()
{
}

HRESULT CSubQuestNode_NoisySnowField01::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CSubQuestNode_NoisySnowField01::Start()
{
	m_pBella = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("AquarisBella"));
	if (m_pBella != nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pBella);

		// 임시로 monster에 
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
	}
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
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
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
		// 퀘스트 테스트용으로 넘겼을 때를 대비
		else if(m_pQuestDestSpot == nullptr)
		{
			m_pBella = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("AquarisBella"));
			if (m_pBella != nullptr) 
			{
				Vec4 vSpotPos = Set_DestSpot(m_pBella);
				m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField01::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField01* CSubQuestNode_NoisySnowField01::Create()
{
	CSubQuestNode_NoisySnowField01* pInstance = new CSubQuestNode_NoisySnowField01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField01::Free()
{
	__super::Free();
}

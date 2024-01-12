#include "stdafx.h"
#include "MainQuestNode_IntroTour01.h"

#include "GameInstance.h"
#include "Utils.h"

CMainQuestNode_IntroTour01::CMainQuestNode_IntroTour01()
{
}

HRESULT CMainQuestNode_IntroTour01::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CMainQuestNode_IntroTour01::Start()
{
	Vec4 vSpotPos = { 0.f, 0.f, 5.f, 1.f };

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CMainQuestNode_IntroTour01::Tick(const _float& fTimeDelta)
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
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_IntroTour01::LateTick(const _float& fTimeDelta)
{
	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
			m_pQuestDestSpot->LateTick(fTimeDelta);
	}
}

CMainQuestNode_IntroTour01* CMainQuestNode_IntroTour01::Create()
{
	CMainQuestNode_IntroTour01* pInstance = new CMainQuestNode_IntroTour01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_IntroTour01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_IntroTour01::Free()
{
	__super::Free();
	Safe_Release(m_pQuestDestSpot);
}

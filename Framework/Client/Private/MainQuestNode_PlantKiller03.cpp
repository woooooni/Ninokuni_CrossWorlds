#include "stdafx.h"
#include "MainQuestNode_PlantKiller03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_PlantKiller03::CMainQuestNode_PlantKiller03()
{
}

HRESULT CMainQuestNode_PlantKiller03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("플랜트 킬러 얻기");
	m_strQuestContent = TEXT("루비에게 부탁해보자");

	return S_OK;
}

void CMainQuestNode_PlantKiller03::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pRuby = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("Ruby"));
	Vec4 vSpotPos = Set_DestSpot(m_pRuby);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));


}

CBTNode::NODE_STATE CMainQuestNode_PlantKiller03::Tick(const _float& fTimeDelta)
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

void CMainQuestNode_PlantKiller03::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_PlantKiller03* CMainQuestNode_PlantKiller03::Create()
{
	CMainQuestNode_PlantKiller03* pInstance = new CMainQuestNode_PlantKiller03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller03::Free()
{
	__super::Free();
}

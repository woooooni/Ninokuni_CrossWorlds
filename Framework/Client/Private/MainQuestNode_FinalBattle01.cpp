#include "stdafx.h"
#include "MainQuestNode_FinalBattle01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"

#include "WitchWood.h"
#include "Moon.h"
#include "UI_PopupQuest.h"

CMainQuestNode_FinalBattle01::CMainQuestNode_FinalBattle01()
{
}

HRESULT CMainQuestNode_FinalBattle01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[¸ÞÀÎ]");
	m_strQuestName = TEXT("µ¢±¼ Á¦°Å");
	m_strQuestContent = TEXT("µ¢±¼¿¡°Ô µû²ûÇÑ ¸ÀÀ» º¸¿©ÁÖÀÚ");

	return S_OK;
}

void CMainQuestNode_FinalBattle01::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::GetInstance()->QUESTEVENT_FINALBATTLE);

	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pWitchWood = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_BUILDING, L"Witch_Wood_Wall");
	m_pMoon = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_SKYBOX, L"Common_Moon");

	Vec4 vSpotPos = { 97.7f, -4.9f, 24.83f, 1.f };
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		// Å×½ºÆ®¿ëÀ¸·Î ÇÑ¹ø ´õ Ã¼Å©
		if (m_pWitchWood == nullptr)
		{
			m_pWitchWood = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_BUILDING, L"Witch_Wood_Wall");
		}
		if (m_pMoon == nullptr)
		{
			// TODO
			m_pMoon = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_SKYBOX, L"Common_Moon");
			static_cast<CMoon*>(m_pMoon)->Set_AppearMoon(true);
		}


		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot->Get_IsCol())
			{
				if (!m_bIsDisAppear)
				{
					m_pWitchWood->Get_Component_StateMachine()->Change_State(CWitchWood::WOOD_STATE::OPEN);
					m_bIsDisAppear = true;
				}
				m_pQuestDestSpot->Set_ReadyDelete(true);
				Safe_Release(m_pQuestDestSpot);
			}
		}
	}

	if (m_bIsDisAppear)
	{
		if (m_pWitchWood->Get_Component_StateMachine()->Get_CurrState() == CWitchWood::WOOD_STATE::OPEN &&
			m_pWitchWood->Get_Component_Model()->Is_Finish() && !m_pWitchWood->Get_Component_Model()->Is_Tween())
		{
			CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
			m_bIsClear = true;

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle01::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_FinalBattle01* CMainQuestNode_FinalBattle01::Create()
{
	CMainQuestNode_FinalBattle01* pInstance = new CMainQuestNode_FinalBattle01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle01::Free()
{
	__super::Free();
}

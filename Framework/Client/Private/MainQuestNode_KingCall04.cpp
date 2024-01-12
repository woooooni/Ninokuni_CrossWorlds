#include "stdafx.h"
#include "MainQuestNode_KingCall04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_KingCall04::CMainQuestNode_KingCall04()
{
}

HRESULT CMainQuestNode_KingCall04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("국왕 루슬란");
	m_strQuestContent = TEXT("루슬란과 대화하기");

	return S_OK;
}

void CMainQuestNode_KingCall04::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_KINGDOMHALL, LAYER_NPC, TEXT("Ruslan"));
	Vec4 vSpotPos = Set_DestSpot(m_pRuslan);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CMainQuestNode_KingCall04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_KINGDOMHALL)
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

void CMainQuestNode_KingCall04::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_KingCall04* CMainQuestNode_KingCall04::Create()
{
	CMainQuestNode_KingCall04* pInstance = new CMainQuestNode_KingCall04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall04::Free()
{
	__super::Free();
}

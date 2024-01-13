#include "stdafx.h"
#include "MainQuestNode_Glanix01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_Glanix01::CMainQuestNode_Glanix01()
{
}

HRESULT CMainQuestNode_Glanix01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("더 깊숙히 조사하기");
	m_strQuestContent = TEXT("코에루크 설원 깊은 곳 까지 들어가보자");

	return S_OK;
}

void CMainQuestNode_Glanix01::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pAren = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("Aren"));
	Vec4 vSpotPos = Set_DestSpot(m_pAren);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CMainQuestNode_Glanix01::Tick(const _float& fTimeDelta)
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
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix01::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Glanix01* CMainQuestNode_Glanix01::Create()
{
	CMainQuestNode_Glanix01* pInstance = new CMainQuestNode_Glanix01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix01::Free()
{
	__super::Free();
}

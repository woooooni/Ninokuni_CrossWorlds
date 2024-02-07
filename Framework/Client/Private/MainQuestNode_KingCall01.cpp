#include "stdafx.h"
#include "MainQuestNode_KingCall01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_KingCall01::CMainQuestNode_KingCall01()
{
}

HRESULT CMainQuestNode_KingCall01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("장교의 시선");
	m_strQuestContent = TEXT("아까부터 나를 지켜본다");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("장교의 시선");
	m_strNextQuestContent = TEXT("지나갈 때마다 시선이 느껴진다");


	return S_OK;
}

void CMainQuestNode_KingCall01::Start()
{
	m_pSanson = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("GrimalKinML02"));
	Vec4 vSpotPos = Set_DestSpot(m_pSanson);

	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
}

CBTNode::NODE_STATE CMainQuestNode_KingCall01::Tick(const _float& fTimeDelta)
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
					CUI_PopupQuest::QUEST_INFO QuestDesc = {};
					QuestDesc.strType = m_strNextQuestTag;
					QuestDesc.strTitle = m_strNextQuestName;
					QuestDesc.strContents = m_strNextQuestContent;
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

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

void CMainQuestNode_KingCall01::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_KingCall01* CMainQuestNode_KingCall01::Create()
{
	CMainQuestNode_KingCall01* pInstance = new CMainQuestNode_KingCall01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall01::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "SubQuestNode_NoisySnowField07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CSubQuestNode_NoisySnowField07::CSubQuestNode_NoisySnowField07()
{
}

HRESULT CSubQuestNode_NoisySnowField07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("시끄러운 코에루코 설원");
	m_strQuestContent = TEXT("벨라에게 돌아가기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("시끄러운 코에루코 설원");
	m_strNextQuestContent = TEXT("벨라에게 돌아가기");


	return S_OK;
}

void CSubQuestNode_NoisySnowField07::Start()
{
//	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strNextQuestTag;
	QuestDesc.strTitle = m_strNextQuestName;
	QuestDesc.strContents = m_strNextQuestContent;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

	m_pBella = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("AquarisBella"));
	Vec4 vSpotPos = Set_DestSpot(m_pBella);

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField07::Tick(const _float& fTimeDelta)
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
					CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
					m_pQuestDestSpot->Set_ReadyDelete(true);
					m_bIsClear = true;
					Safe_Release(m_pQuestDestSpot);

					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField07::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField07* CSubQuestNode_NoisySnowField07::Create()
{
	CSubQuestNode_NoisySnowField07* pInstance = new CSubQuestNode_NoisySnowField07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField07::Free()
{
	__super::Free();
}

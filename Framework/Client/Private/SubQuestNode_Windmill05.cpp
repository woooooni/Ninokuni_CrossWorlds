#include "stdafx.h"
#include "SubQuestNode_Windmill05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CSubQuestNode_Windmill05::CSubQuestNode_Windmill05()
{
}

HRESULT CSubQuestNode_Windmill05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[¼­ºê]");
	m_strQuestName = TEXT("Ç³Â÷ ¼ö¸®");
	m_strQuestContent = TEXT("±«Â¥ ¼Ò³à¸¦ Ã£±â");

	m_strNextQuestTag = TEXT("[¼­ºê]");
	m_strNextQuestName = TEXT("Ç³Â÷ ¼ö¸®");
	m_strNextQuestContent = TEXT("±«Â¥ ¼Ò³à¸¦ Ã£±â");

	return S_OK;
}

void CSubQuestNode_Windmill05::Start()
{
	m_pGeekGirl = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Engineer_Dummy"));
	Vec4 vSpotPos = Set_DestSpot(m_pGeekGirl);

	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Windmill05::Tick(const _float& fTimeDelta)
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
//					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

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

void CSubQuestNode_Windmill05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill05* CSubQuestNode_Windmill05::Create()
{
	CSubQuestNode_Windmill05* pInstance = new CSubQuestNode_Windmill05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill05::Free()
{
	__super::Free();
}

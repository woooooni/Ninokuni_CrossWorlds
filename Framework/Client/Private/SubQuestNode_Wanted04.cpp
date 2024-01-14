#include "stdafx.h"
#include "SubQuestNode_Wanted04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted04::CSubQuestNode_Wanted04()
{
}

HRESULT CSubQuestNode_Wanted04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[¼­ºê]");
	m_strQuestName = TEXT("µµµÏ Àâ±â");
	m_strQuestContent = TEXT("¿Õ±¹ÀÇ µµµÏ³ðÀ» Àâ¾Æ¶ó.");

	m_strNextQuestTag = TEXT("[¼­ºê]");
	m_strNextQuestName = TEXT("µµµÏ Àâ±â");
	m_strNextQuestContent = TEXT("¿Õ±¹ÀÇ µµµÏ³ðÀ» Àâ¾Æ¶ó.");

	return S_OK;
}

void CSubQuestNode_Wanted04::Start()
{	
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestTag, m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CSubQuestNode_Wanted04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* ÀÓ½Ã */
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	/* ÃßÈÄ Çö»ó¹ü Á×ÀÌ¸é. */
	//if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	//{
	//	if (m_pQuestDestSpot != nullptr)
	//	{
	//		m_pQuestDestSpot->Tick(fTimeDelta);
	//		m_pQuestDestSpot->LateTick(fTimeDelta);
	//
	//		if (m_pQuestDestSpot != nullptr)
	//		{
	//			if (m_pQuestDestSpot->Get_IsCol())
	//			{
	//				m_bIsClear = true;
	//				m_pQuestDestSpot->Set_Dead(true);
	//				return NODE_STATE::NODE_FAIL;
	//			}
	//		}
	//	}
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted04::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted04* CSubQuestNode_Wanted04::Create()
{
	CSubQuestNode_Wanted04* pInstance = new CSubQuestNode_Wanted04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted04::Free()
{
	__super::Free();
}

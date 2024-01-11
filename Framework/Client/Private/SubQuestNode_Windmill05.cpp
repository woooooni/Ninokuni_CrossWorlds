#include "stdafx.h"
#include "SubQuestNode_Windmill05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Windmill05::CSubQuestNode_Windmill05()
{
}

HRESULT CSubQuestNode_Windmill05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("풍차 수리");
	m_strQuestContent = TEXT("미정(뭔가하겠지)");

	return S_OK;
}

void CSubQuestNode_Windmill05::Start()
{
	//CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	//m_pChloe = dynamic_cast<CChloe*>(GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Chloe")));
	//Vec4 vSpotPos = Set_DestSpot(m_pChloe);
	//
	//// 임시로 monster에 
	//m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Windmill05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// 미정 퀘스트 완료해야 하나 임시로 일단 키 입력으로 넘어가자.
	if (KEY_TAP(KEY::N))
	{
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
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

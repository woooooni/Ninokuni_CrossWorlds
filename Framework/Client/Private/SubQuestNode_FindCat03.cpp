#include "stdafx.h"
#include "SubQuestNode_FindCat03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CSubQuestNode_FindCat03::CSubQuestNode_FindCat03()
{
}

HRESULT CSubQuestNode_FindCat03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("감귤이 찾기");
	m_strQuestContent = TEXT("감귤이를 찾아주자");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("감귤이 찾기");
	m_strNextQuestContent = TEXT("감귤이를 찾아주자");

	return S_OK;
}

void CSubQuestNode_FindCat03::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
}

CBTNode::NODE_STATE CSubQuestNode_FindCat03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	m_bIsClear = true;

	// 고양이를 찾아야 하나 임시로 일단 키 입력으로 넘어가자.
	//if (KEY_TAP(KEY::N))
	//{
	//	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
	//
	//	m_bIsClear = true;
	//	return NODE_STATE::NODE_FAIL;
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_FindCat03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_FindCat03* CSubQuestNode_FindCat03::Create()
{
	CSubQuestNode_FindCat03* pInstance = new CSubQuestNode_FindCat03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_FindCat03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_FindCat03::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "SubQuestNode_Windmill07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

CSubQuestNode_Windmill07::CSubQuestNode_Windmill07()
{
}

HRESULT CSubQuestNode_Windmill07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("비행기 배틀!");
	m_strQuestContent = TEXT("비행기 배틀로 소녀를 혼내주자");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("비행기 배틀!");
	m_strNextQuestContent = TEXT("비행기 배틀로 소녀를 혼내주자");

	return S_OK;
}

void CSubQuestNode_Windmill07::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);
}

CBTNode::NODE_STATE CSubQuestNode_Windmill07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// 여기서 게임 로직.
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill07::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill07* CSubQuestNode_Windmill07::Create()
{
	CSubQuestNode_Windmill07* pInstance = new CSubQuestNode_Windmill07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill07::Free()
{
	__super::Free();
}

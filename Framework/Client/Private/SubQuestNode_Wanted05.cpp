#include "stdafx.h"
#include "SubQuestNode_Wanted05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted05::CSubQuestNode_Wanted05()
{
}

HRESULT CSubQuestNode_Wanted05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("도둑 잡기");
	m_strQuestContent = TEXT("왕국의 도둑놈을 잡아라.");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("툼바에게 돌아가기");
	m_strNextQuestContent = TEXT("툼바에게 보고하자");

	return S_OK;
}

void CSubQuestNode_Wanted05::Start()
{
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestTag, m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* 추후 현상범 잡고나면 뭐 할까? 경비병에게 인계? */
}

CBTNode::NODE_STATE CSubQuestNode_Wanted05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* 추후 현상범 잡고나면 뭐 할까? 경비병에게 인계? */
	/* 임시 */
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}


	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted05* CSubQuestNode_Wanted05::Create()
{
	CSubQuestNode_Wanted05* pInstance = new CSubQuestNode_Wanted05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted05::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "SubQuestNode_NoisySnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"

#include "CurlingGame_Group.h"

CSubQuestNode_NoisySnowField05::CSubQuestNode_NoisySnowField05()
{
}

HRESULT CSubQuestNode_NoisySnowField05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("시끄러운 코에루코 설원");
	m_strQuestContent = TEXT("대결에서 승리하여 진정시키자");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("시끄러운 코에루코 설원");
	m_strNextQuestContent = TEXT("대결에서 승리하여 진정시키자");

	return S_OK;
}

void CSubQuestNode_NoisySnowField05::Start()
{
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

	// 게임 시작 함수 추가
	CCurlingGame_Manager::GetInstance()->Start_Game();

	// 페이드 인
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 2.f);
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;
	
	if (CCurlingGame_Manager::GetInstance()->Finish_Game()) // 만약 게임이 끝났다면 
	{
		// 페이드는 컬링 게임 매니저 쪽에서 처리한다. 
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;

		//// 페이드 아웃 시작
		//if (!m_bFadeOut)
		//{
		//	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 3.f);
		//	m_bFadeOut = true;
		//}
	}

	// 페이드 아웃이 끝났다면 다음 노드로 넘어간다.
	//if (m_bFadeOut)
	//{
	//	if (CUI_Manager::GetInstance()->Is_FadeFinished())
	//	{
	//		m_bIsClear = true;
	//		return NODE_STATE::NODE_FAIL;
	//	}
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField05* CSubQuestNode_NoisySnowField05::Create()
{
	CSubQuestNode_NoisySnowField05* pInstance = new CSubQuestNode_NoisySnowField05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField05::Free()
{
	__super::Free();
}

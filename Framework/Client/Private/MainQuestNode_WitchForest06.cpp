#include "stdafx.h"
#include "MainQuestNode_WitchForest06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "WitchWood.h"

CMainQuestNode_WitchForest06::CMainQuestNode_WitchForest06()
{
}

HRESULT CMainQuestNode_WitchForest06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("악몽의 마녀에게");
	m_strQuestContent = TEXT("마을을 나가 오른쪽으로 가기");

	return S_OK;
}

void CMainQuestNode_WitchForest06::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
//	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
	
	m_pWitchWood = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_BUILDING, L"Witch_Wood_Wall");
}

CBTNode::NODE_STATE CMainQuestNode_WitchForest06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pWitchWood->Get_Component_StateMachine()->Get_CurrState() == CWitchWood::WOOD_STATE::CLOSE &&
		m_pWitchWood->Get_Component_Model()->Is_Finish() && ! m_pWitchWood->Get_Component_Model()->Is_Tween())
	{
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest06::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_WitchForest06* CMainQuestNode_WitchForest06::Create()
{
	CMainQuestNode_WitchForest06* pInstance = new CMainQuestNode_WitchForest06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_WitchForest06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_WitchForest06::Free()
{
	__super::Free();
}

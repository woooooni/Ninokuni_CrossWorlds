#include "stdafx.h"
#include "SubQuestNode_Windmill11.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Building.h"

CSubQuestNode_Windmill11::CSubQuestNode_Windmill11()
{
}

HRESULT CSubQuestNode_Windmill11::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("풍차 수리");
	m_strQuestContent = TEXT("베르디에게 가기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("풍차 수리");
	m_strNextQuestContent = TEXT("베르디에게 돌아가기");

	return S_OK;
}

void CSubQuestNode_Windmill11::Start()
{
	/* 풍차 비춰주기 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
	{
		pActionCam->Start_Action_WindMill(true);

		CGameObject* pGameObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_BUILDING, L"Evermore_Wind_WindMillaA_02");
		if (nullptr != pGameObject)
		{
			CBuilding* pWindMill = dynamic_cast<CBuilding*>(pGameObject);
			if (nullptr != pWindMill)
			{
				pWindMill->Set_QuestClear(true);
			}
		}
	}
}

CBTNode::NODE_STATE CSubQuestNode_Windmill11::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* 풍차 비춰주기가 끝났다면 다음으로 넘어간다. */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
	{
		if (CCamera_Action::CAMERA_ACTION_TYPE::WINDMILL != pActionCam->Get_Camera_ActionType())
		{
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill11::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill11* CSubQuestNode_Windmill11::Create()
{
	CSubQuestNode_Windmill11* pInstance = new CSubQuestNode_Windmill11();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill11");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill11::Free()
{
	__super::Free();
}

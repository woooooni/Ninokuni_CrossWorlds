#include "stdafx.h"
#include "MainQuestNode_Invasion02_3.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_Invasion02_3::CMainQuestNode_Invasion02_3()
{
}

HRESULT CMainQuestNode_Invasion02_3::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CMainQuestNode_Invasion02_3::Start()
{
	/* 파괴된 에스타나비아 비춰주기 */
	//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	//if (nullptr != pActionCam)
	//{
	//	pActionCam->Start_Action_WindMill(true);
	//
	//	CGameObject* pGameObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_BUILDING, L"Evermore_Wind_WindMillaA_02");
	//	if (nullptr != pGameObject)
	//	{
	//		CBuilding* pWindMill = dynamic_cast<CBuilding*>(pGameObject);
	//		if (nullptr != pWindMill)
	//		{
	//			pWindMill->Set_QuestClear(true);
	//		}
	//	}
	//}
}

CBTNode::NODE_STATE CMainQuestNode_Invasion02_3::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	m_bIsClear = true;

	/* 파괴된 에스타나비아 비춰주기가 끝났다면 다음으로 넘어간다. */
	//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	//if (nullptr != pActionCam)
	//{
	//	if (CCamera_Action::CAMERA_ACTION_TYPE::WINDMILL != pActionCam->Get_Camera_ActionType())
	//	{
	//		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
	//		m_bIsClear = true;
	//		return NODE_STATE::NODE_FAIL;
	//	}
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion02_3::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Invasion02_3* CMainQuestNode_Invasion02_3::Create()
{
	CMainQuestNode_Invasion02_3* pInstance = new CMainQuestNode_Invasion02_3();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion02_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion02_3::Free()
{
	__super::Free();
}

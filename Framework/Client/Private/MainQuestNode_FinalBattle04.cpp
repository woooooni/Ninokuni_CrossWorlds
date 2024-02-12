#include "stdafx.h"
#include "MainQuestNode_FinalBattle04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Quest_Manager.h"

#include "Stellia.h"

#include "Camera_Group.h"

CMainQuestNode_FinalBattle04::CMainQuestNode_FinalBattle04()
{
}

HRESULT CMainQuestNode_FinalBattle04::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CMainQuestNode_FinalBattle04::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia"), nullptr, &m_pStellia)))
	{
		MSG_BOX("Fail AddGameObj : Quest Stellia");
	}

	/* 스텔리아 등장 컷신 호출 */
	{
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
			pActionCam->Start_Action_Stellia_Roar(m_pStellia);

		CCamera_Manager::GetInstance()->Set_CurCamera(pActionCam->Get_Key());
	}

	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_BOSS_KILL);
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pStellia != nullptr)
	{
		if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_SPAWNSTAND)
		{
			m_bIsClear = true;

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle04::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_FinalBattle04* CMainQuestNode_FinalBattle04::Create()
{
	CMainQuestNode_FinalBattle04* pInstance = new CMainQuestNode_FinalBattle04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle04::Free()
{
	__super::Free();
}

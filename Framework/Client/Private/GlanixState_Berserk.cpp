#include "stdafx.h"
#include "GlanixState_Berserk.h"

#include "Glanix.h"

#include "Camera_Manager.h"
#include "Camera_CutScene_Boss.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Fade.h"
#include "Game_Manager.h"
#include "Player.h"

CGlanixState_Berserk::CGlanixState_Berserk(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Berserk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fFadeOutTime = 0.75f;
	m_fFadeInTime = 0.75f;

	return S_OK;
}

void CGlanixState_Berserk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));

	/* Camera (Start CutScene) */
	CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
	if (nullptr != pCutSceneCam)
	{
		if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::CUTSCENE_BOSS)))
			return;

		if (FAILED(pCutSceneCam->Start_CutScene(
			CCamera_CutScene_Boss::BOSS_TYPE::GLANIX,
			CCamera_CutScene_Boss::GLANIX_CUTSCENE_TYPE::PAGE,
			m_pGlanix)))
			return;

		/* Ui Off */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		/* Player Input Off */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
	}

}

void CGlanixState_Berserk::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}

	if (!m_bCheck && !m_pModelCom->Is_Tween() && 70 == m_pModelCom->Get_CurrAnimationFrame())
	{
		m_bCheck = true;
		CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
		if (nullptr != pCutSceneCam)
			pCutSceneCam->Send_Signal();
	}

	if (!m_pModelCom->Is_Tween() && 0.95f <= m_pModelCom->Get_Progress() && !m_bFadeOut)
	{
		/* Start Fade Out */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_fFadeOutTime, false);

		m_bFadeOut = true;
	}
}

void CGlanixState_Berserk::Exit_State()
{
	/* Camera */
	CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
	if (nullptr != pCutSceneCam)
	{
		/* Start Fade In */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_fFadeInTime, false);

		/* On UI */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);

		/* Finish CutScene */
		if (FAILED(pCutSceneCam->Finish_CutScene()))
			return;

		/* Player Input On */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
	}

	m_bCheck = false;
	m_bFadeOut = false;
}

CGlanixState_Berserk* CGlanixState_Berserk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Berserk* pInstance = new CGlanixState_Berserk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Berserk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Berserk::Free()
{
	__super::Free();
}

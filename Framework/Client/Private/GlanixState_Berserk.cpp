#include "stdafx.h"
#include "GlanixState_Berserk.h"

#include "Glanix.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Fade.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"
#include "Transform.h"
#include "Effect_Manager.h"

CGlanixState_Berserk::CGlanixState_Berserk(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Berserk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fFadeOutTime = 0.75f;
	m_fFadeInTime = 0.75f;

	m_strCutSceneNames.push("Winter_Boss_Page_00");
	m_strCutSceneNames.push("Winter_Boss_Page_01");

	return S_OK;
}

void CGlanixState_Berserk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));

	CCamera_CutScene_Map* pCutSceneCam = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
	if (nullptr != pCutSceneCam)
	{
		/* Change Camera */
		if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::CUTSCENE_BOSS)))
			return;

		/* Start CutScene */
		if (FAILED(pCutSceneCam->Start_CutScene(m_strCutSceneNames.front(), false)))
			return;
		m_strCutSceneNames.pop();

		/* Ui Off */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		/* Player Input Off */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);

		/* Reset Glanix Position */
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, m_pGlanix->Get_OriginPos());
		m_pTransformCom->LookAt_ForLandObject(Vec4::UnitW);

		pCutSceneCam->Start_Lerp_Fov(Cam_Fov_CutScene_Map_Default, XMConvertToRadians(60.f), 2.5f, LERP_MODE::SMOOTHER_STEP);
	}

	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Intro_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pGlanix);
}

void CGlanixState_Berserk::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
		//m_pStateMachineCom->Change_State(CGlanix::GLANIX_BERSERK);
	}

	if (!m_bCheck && !m_pModelCom->Is_Tween() && 68 == m_pModelCom->Get_CurrAnimationFrame())
	{
		m_bCheck = true;

		CCamera_CutScene_Map* pCutSceneCam = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
		if (nullptr != pCutSceneCam)
		{
			if (FAILED(pCutSceneCam->Start_CutScene(m_strCutSceneNames.front(), false)))
				return;

			m_strCutSceneNames.pop();

			pCutSceneCam->Start_Lerp_Fov(XMConvertToRadians(40.f), XMConvertToRadians(60.f), 0.75f, LERP_MODE::EASE_OUT);
		}
	}

	if (!m_pModelCom->Is_Tween() && 0.80f <= m_pModelCom->Get_Progress() && !m_bFadeOut)
	{
		/* Start Fade Out */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_fFadeOutTime, false);

		m_bFadeOut = true;
	}
}

void CGlanixState_Berserk::Exit_State()
{	
	/* Change Camera */
	CCamera_CutScene_Map* pCutSceneCam = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
	if (nullptr != pCutSceneCam)
	{
		if (FAILED(pCutSceneCam->Stop_CutScene()))
			return;

		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
			pFollowCam->Set_Default_Position();

		if(FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(pFollowCam->Get_Key())))
			return;		
	}

	/* Start Fade In */
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_fFadeInTime, false);

	/* On UI */
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);

	/* Player Input On */
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
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

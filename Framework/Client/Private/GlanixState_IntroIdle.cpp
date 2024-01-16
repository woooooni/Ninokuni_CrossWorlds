#include "stdafx.h"
#include "GlanixState_IntroIdle.h"

#include "Glanix.h"

#include "Camera_Manager.h"
#include "Camera_CutScene_Boss.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Fade.h"


CGlanixState_IntroIdle::CGlanixState_IntroIdle(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_IntroIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fWaitTime = 5.f;
	m_fFadeOutTime = 0.75f;
	m_fFadeInTime = 0.75f;

	m_bFadeIn = false;

	return S_OK;
}

void CGlanixState_IntroIdle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;
	m_bFadeIn = false;

	/* Start Fade Out */
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_fFadeOutTime, false);

	/* Change Bgm */
	GI->Play_BGM(L"BGM_Field_BridgeCave_Ne_1.ogg", GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), true);
}

void CGlanixState_IntroIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fTime += fTimeDelta;

	/* Finish Fade Out */
	if (m_fTime >= m_fFadeOutTime && !m_bFadeIn)
	{
		m_bFadeIn = true;

		/* Camera (Start CutScene) */
		CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
		if (nullptr != pCutSceneCam)
		{
			if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::CUTSCENE_BOSS)))
				return;

			if (FAILED(pCutSceneCam->Start_CutScene(
				CCamera_CutScene_Boss::BOSS_TYPE::GLANIX,
				CCamera_CutScene_Boss::GLANIX_CUTSCENE_TYPE::APPEAR,
				m_pGlanix)))
				return;
		}

		/* Start Fade In */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_fFadeInTime, false);

		/* Ui Off */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);
	}

	/* Change State */
	if (m_fTime >= m_fWaitTime)
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_INTRO_ROAR);
	}
}

void CGlanixState_IntroIdle::Exit_State()
{
}

CGlanixState_IntroIdle* CGlanixState_IntroIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_IntroIdle* pInstance = new CGlanixState_IntroIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_IntroIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_IntroIdle::Free()
{
	__super::Free();
}


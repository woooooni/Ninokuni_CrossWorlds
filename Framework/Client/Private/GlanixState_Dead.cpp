#include "stdafx.h"
#include "GlanixState_Dead.h"

#include "Glanix.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "UI_Fade.h"

#include "GameInstance.h"

CGlanixState_Dead::CGlanixState_Dead(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Death"));
}

void CGlanixState_Dead::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if(CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::QUESTEVENT_BOSS_KILL)
			CQuest_Manager::GetInstance()->Set_IsBossKill(true);

		m_pGlanix->Reserve_Dead(true);

		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam && pFollowCam->Is_LockOn())
		{
			pFollowCam->Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
			pFollowCam->Reset_WideView_To_DefaultView(true);
			pFollowCam->Set_Default_Position();

			m_pGlanix->Reserve_Dead(true);
			GI->Play_BGM(TEXT("BGM_Field_Village_Winter_Po_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, 0.5f);
		}
	}

	//if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	//{
	//	/* Start Fade Out */
	//	if (!m_bStartFadeOut)
	//	{
	//		m_bStartFadeOut = true;
	//		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_fFadeInOutTime, false);
	//	}
	//}

	//if (m_bStartFadeOut && CUI_Manager::GetInstance()->Is_FadeFinished())
	//{
	//	/* Reset Camera */
	//	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	//	if (nullptr != pFollowCam && pFollowCam->Is_LockOn())
	//	{
	//		pFollowCam->Set_Default_Position();
	//		pFollowCam->Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	//	}

	//	m_fAcc += fTimeDelta;
	//		
	//	/* Start Fade In */
	//	if (!m_bStartFadeIn && m_fFadeInOutTime <= m_fAcc)
	//	{
	//		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, m_fFadeInOutTime, false);
	//		m_bStartFadeIn = true;

	//		m_pGlanix->Reserve_Dead(true);
	//	}
	//}
}

void CGlanixState_Dead::Exit_State()
{
}

CGlanixState_Dead* CGlanixState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Dead* pInstance = new CGlanixState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Dead::Free()
{
	__super::Free();
}

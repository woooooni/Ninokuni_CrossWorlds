#include "stdafx.h"
#include "GlanixState_Rage2Rising.h"

#include "Glanix.h"

#include "Animation.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CGlanixState_Rage2Rising::CGlanixState_Rage2Rising(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Rising::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fFlyTime = 2.f;

	return S_OK;
}

void CGlanixState_Rage2Rising::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalJump"));

	m_fTime = 0.f;

	/* Camera */
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	if (nullptr != pFollowCam)
	{
		/* 락온 + 와이드뷰 Off */

		pFollowCam->Start_Lerp_Fov(Cam_Fov_Follow_Default,
			Cam_LerpTime_LockOn_Glanix_ItemPattern_Blending_Out,
			LERP_MODE::SMOOTHER_STEP);

		pFollowCam->Lerp_TargetOffset(pFollowCam->Get_TargetOffset(),
			Cam_Target_Offset_LockOn_Glanix,
			Cam_LerpTime_LockOn_Glanix_ItemPattern_Blending_Out,
			LERP_MODE::SMOOTHER_STEP);

		// 플레이어 공격 인풋 열기
		//CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Attack_Input(true);
		//CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Skill_Input(true);
	}
}

void CGlanixState_Rage2Rising::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if(!m_pModelCom->Is_Tween() && 24 <= m_pModelCom->Get_CurrAnimationFrame())
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), 40.f, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2STAMP);
	}

	/* Camera */
	if (20 == m_pModelCom->Get_CurrAnimationFrame())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}
	}
}

void CGlanixState_Rage2Rising::Exit_State()
{
}

CGlanixState_Rage2Rising* CGlanixState_Rage2Rising::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Rising* pInstance = new CGlanixState_Rage2Rising(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Rising");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Rising::Free()
{
	__super::Free();
}


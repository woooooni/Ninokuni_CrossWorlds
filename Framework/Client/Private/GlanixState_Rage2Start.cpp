#include "stdafx.h"
#include "GlanixState_Rage2Start.h"

#include "Glanix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CGlanixState_Rage2Start::CGlanixState_Rage2Start(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Start::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Rage2Start::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));

	/* Camera */
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	if (nullptr != pFollowCam)
	{
		/* 락온 + 와이드뷰 Start */

		pFollowCam->Start_Lerp_Fov(Cam_Fov_LockOn_Glanix_ItemPattern, 
									Cam_LerpTime_LockOn_Glanix_ItemPattern_Blending_In,
									LERP_MODE::SMOOTHER_STEP);

		pFollowCam->Lerp_TargetOffset(pFollowCam->Get_TargetOffset(), 
										Cam_Target_Offset_LockOn_Glanix_ItemPattern, 
										Cam_LerpTime_LockOn_Glanix_ItemPattern_Blending_In,
										LERP_MODE::SMOOTHER_STEP);

		// 플레이어 공격 인풋 막기
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Attack_Input(false);
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Skill_Input(false);
	}
}

void CGlanixState_Rage2Start::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2WAVE);
	}
}

void CGlanixState_Rage2Start::Exit_State()
{
	//_float4 vOwnerPos = {};
	//XMStoreFloat4(&vOwnerPos, m_pGlanix->Get_OriginPos());
	//_vector vSpritPos = { vOwnerPos.x, vOwnerPos.y, vOwnerPos.z, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_Phoenix"));
}

CGlanixState_Rage2Start* CGlanixState_Rage2Start::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Start* pInstance = new CGlanixState_Rage2Start(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Start::Free()
{
	__super::Free();
}

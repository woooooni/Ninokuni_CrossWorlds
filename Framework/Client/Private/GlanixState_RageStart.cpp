#include "stdafx.h"
#include "GlanixState_RageStart.h"

#include "Glanix.h"

#include "GameInstance.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

CGlanixState_RageStart::CGlanixState_RageStart(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
}

void CGlanixState_RageStart::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageStart::Exit_State()
{
	//_float4 vGlanixPos = {};
	//XMStoreFloat4(&vGlanixPos, m_pTransformCom->Get_Position());
	//
	//_vector vPillarPos = { vGlanixPos.x + 8.f, 1.6f, vGlanixPos.z + -8.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	//vPillarPos = { vGlanixPos.x + -15.f, 1.6f, vGlanixPos.z + -8.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	//vPillarPos = { vGlanixPos.x + 12.f, 1.6f, vGlanixPos.z + 11.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	//vPillarPos = { vGlanixPos.x + -11.f, 1.6f, vGlanixPos.z + 13.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);

	_vector vPillarPos = { -64.f, 10.f, 360.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -34.f, 10.f, 350.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -60.f, 10.f, 334.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -36.f, 10.f, 378.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -63.f, 10.f, 378.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -44.f, 10.f, 391.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -23.f, 10.f, 363.f, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);

	/* Camera */
	//CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	//if (nullptr != pFollowCam)
	//{
	//	/* 락온 + 와이드뷰 Start */
	//
	//	pFollowCam->Start_Lerp_Fov(Cam_Fov_LockOn_Glanix_PillarPattern,
	//		Cam_LerpTime_LockOn_Glanix_PillarPattern,
	//		LERP_MODE::SMOOTHER_STEP);
	//
	//	pFollowCam->Lerp_TargetOffset(pFollowCam->Get_TargetOffset(),
	//		Cam_Target_Offset_LockOn_Glanix_PillarPattern,
	//		Cam_LerpTime_LockOn_Glanix_PillarPattern,
	//		LERP_MODE::SMOOTHER_STEP);
	//
	//	// 플레이어 공격 인풋 막기
	//}
}

CGlanixState_RageStart* CGlanixState_RageStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStart* pInstance = new CGlanixState_RageStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStart::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GlanixState_RageStamp.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "GameInstance.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

CGlanixState_RageStamp::CGlanixState_RageStamp(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalStamp"));
}

void CGlanixState_RageStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 15)
	{
		/* 남아있는 얼음기둥 제거 */
		for (auto iter : GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_PROP))
		{
			if (iter->Get_PrototypeTag() == TEXT("Prorotype_GameObject_Glanix_IcePillar"))
			{
				iter->Set_Dead(true);
			}
		}

		/* Camera */
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam)
		{
			/* 락온 + 와이드뷰 Off */
		
			pFollowCam->Start_Lerp_Fov(Cam_Fov_Follow_Default,
				Cam_LerpTime_LockOn_Glanix_PillarPattern,
				LERP_MODE::SMOOTHER_STEP);
		
			pFollowCam->Lerp_TargetOffset(pFollowCam->Get_TargetOffset(),
				Cam_Target_Offset_LockOn_Glanix,
				Cam_LerpTime_LockOn_Glanix_PillarPattern,
				LERP_MODE::SMOOTHER_STEP);
		
			// 플레이어 공격 인풋 열기
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_STUN);
	}
}

void CGlanixState_RageStamp::Exit_State()
{
}

CGlanixState_RageStamp* CGlanixState_RageStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStamp* pInstance = new CGlanixState_RageStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStamp::Free()
{
	__super::Free();
}


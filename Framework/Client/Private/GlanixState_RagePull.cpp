#include "stdafx.h"
#include "GlanixState_RagePull.h"

#include "Glanix.h"

#include "Animation.h"

#include "Camera_Manager.h"
#include "Camera.h"

#include "Effect_Manager.h"

CGlanixState_RagePull::CGlanixState_RagePull(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RagePull::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RagePull::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageSkillPull"));

	/* 남아있는 얼음기둥 제거 */
	m_pGlanix->Clear_Pillars();

	/* 카메라 줌 */
	CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
	if (nullptr != pCurCam)
	{
		pCurCam->Lerp_TargetOffset(pCurCam->Get_TargetOffset(), Cam_TargetOffset_Top_Glanix_Zoom, 1.f, LERP_MODE::SMOOTHER_STEP);
		pCurCam->Lerp_LookAtOffSet(pCurCam->Get_LookAtOffset(), Cam_LookAtOffset_Top_Glanix_Zoom, 1.f, LERP_MODE::SMOOTHER_STEP);
	}
}

void CGlanixState_RagePull::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 20 && m_pModelCom->Get_CurrAnimationFrame() <= 175)
	{
		_vector vPullDir = XMVector3Normalize(m_pTransformCom->Get_Position() - m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position());
		m_pGlanix->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(vPullDir, 10.5f, true);

		if (m_pModelCom->Get_CurrAnimationFrame() <= 165)
		{
			GET_INSTANCE(CEffect_Manager)->Tick_Generate_Effect(&fEffectTimeAcc, 0.4f, fTimeDelta, TEXT("Effect_Glanix_Roar_TrailLine_InMoveCircleLine"),
				m_pTransformCom->Get_WorldMatrix(), _float3(0.f, -0.1f, 0.f), _float3(15.f, 10.f, 15.f), _float3(180.f, 0.f, 0.f));
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGERISING);
	}
}

void CGlanixState_RagePull::Exit_State()
{
}

CGlanixState_RagePull* CGlanixState_RagePull::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RagePull* pInstance = new CGlanixState_RagePull(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RagePull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RagePull::Free()
{
	__super::Free();
}


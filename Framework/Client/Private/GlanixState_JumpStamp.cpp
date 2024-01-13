#include "stdafx.h"
#include "GlanixState_JumpStamp.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "Camera_Follow.h"
#include "Effect_Manager.h"


CGlanixState_JumpStamp::CGlanixState_JumpStamp(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_JumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_JumpStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill03"));

	// Effect Create
	CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_JumpDown"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
}

void CGlanixState_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if(m_pModelCom->Get_CurrAnimationFrame() < 45)
		vDestPos = m_pPlayerTransform->Get_Position();

	if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && m_pModelCom->Get_CurrAnimationFrame() <= 75)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		Vec4 vFinalPosition = vDestPos;
		Vec3 vReverseDir = XMVector3Normalize(m_pTransformCom->Get_Position() - vDestPos);
		vFinalPosition = vFinalPosition + vReverseDir * 1.5f;

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vFinalPosition, fTimeDelta / 0.35f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}

	/* Camera */
	if (45 == m_pModelCom->Get_CurrAnimationFrame())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}
	}
}

void CGlanixState_JumpStamp::Exit_State()
{
}

CGlanixState_JumpStamp* CGlanixState_JumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_JumpStamp* pInstance = new CGlanixState_JumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_JumpStamp::Free()
{
	__super::Free();
}


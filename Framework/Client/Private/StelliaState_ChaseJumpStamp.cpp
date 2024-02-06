#include "stdafx.h"
#include "StelliaState_ChaseJumpStamp.h"

#include "Stellia.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "Camera_Follow.h"
#include "Effect_Manager.h"


CStelliaState_ChaseJumpStamp::CStelliaState_ChaseJumpStamp(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_ChaseJumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_ChaseJumpStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillChaseJump"));

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_ChaseJumpStamp"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_ChaseJumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() < 29)
		vDestPos = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position();

	if (m_pModelCom->Get_CurrAnimationFrame() >= 29 && m_pModelCom->Get_CurrAnimationFrame() <= 60)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		Vec4 vFinalPosition = vDestPos;
		Vec3 vReverseDir = XMVector3Normalize(m_pTransformCom->Get_Position() - vDestPos);
		vFinalPosition = vFinalPosition + vReverseDir * 3.f;

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vFinalPosition, fTimeDelta / 0.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE, &fWaitTime);
	}

	/* Camera */
	//if (29 == m_pModelCom->Get_CurrAnimationFrame())
	//{
	//	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	//	if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
	//	{
	//		pFollowCam->Lock_LookHeight();
	//	}
	//}
}

void CStelliaState_ChaseJumpStamp::Exit_State()
{
}

CStelliaState_ChaseJumpStamp* CStelliaState_ChaseJumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_ChaseJumpStamp* pInstance = new CStelliaState_ChaseJumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_ChaseJumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_ChaseJumpStamp::Free()
{
	__super::Free();
}


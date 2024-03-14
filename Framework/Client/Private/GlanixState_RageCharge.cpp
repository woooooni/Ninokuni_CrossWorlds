#include "stdafx.h"
#include "GlanixState_RageCharge.h"

#include "Glanix.h"
#include "Camera_Manager.h"

CGlanixState_RageCharge::CGlanixState_RageCharge(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageCharge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChargeSpeed = 20.f;
	// m_fChargeTime = 1.5f;

	m_fCurDist = 0.f; // 26
	m_fPrevDist = 0.f; // 25

	return S_OK;
}

void CGlanixState_RageCharge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageSkillCharge"));

	if (m_iChargeCount >= m_iMaxChargeCount)
	{
		m_iChargeCount = 0;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGERETURN);

		return;
	}

	//if (!m_bIsOut && fabs(m_fCurDist) >= 25.f)
	//{
	//
	//}

	// m_fPrevDist = m_fCurDist; 

	m_fTime = 0.f;
}

void CGlanixState_RageCharge::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimationFrame() <= 45)
	{
		m_pTransformCom->LookAt_ForLandObject(m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position());
	}

	else if (m_pModelCom->Get_CurrAnimationFrame() >= 50)
	{
		/* 여기서 충돌 했을 때 Crash로 전환하면서 Crash 카운트 추가. */
		if (m_pGlanix->Get_IsCrash())
		{
			m_pGlanix->Set_IsCrash(false);
			m_iChargeCount += 1;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGECRASH);
			return;
		}

		/* 계산 */
		if (!m_bIsOut && fabs(m_fCurDist) >= 25.f)
		{
			m_bIsOut = true;
			m_iChargeCount += 1;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGECHARGE_END);
			return;
		}

		m_vCurPos = m_pTransformCom->Get_Position();

		// m_fPrevDist = m_fCurDist;
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed, fTimeDelta);
		m_fCurDist = Vec4(m_vCurPos - m_pGlanix->Get_OriginPos()).Length();

		if (m_bIsOut && fabs(m_fCurDist) < 22.f)
			m_bIsOut = false;

		// m_fTime += fTimeDelta;

		//if (m_fTime >= m_fChargeTime)
		//{
		//if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		//{
		//	m_iChargeCount += 1;
		//	m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGECHARGE_END);
		//}
		//}
	}

	//if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	//{
	//	m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	//}
}

void CGlanixState_RageCharge::Exit_State()
{
}

CGlanixState_RageCharge* CGlanixState_RageCharge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageCharge* pInstance = new CGlanixState_RageCharge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageCharge::Free()
{
	__super::Free();
}

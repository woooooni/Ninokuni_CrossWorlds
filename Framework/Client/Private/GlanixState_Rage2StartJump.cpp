#include "stdafx.h"
#include "GlanixState_Rage2StartJump.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"

CGlanixState_Rage2StartJump::CGlanixState_Rage2StartJump(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2StartJump::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Rage2StartJump::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill03"));

	vDestPos = m_pGlanix->Get_WavePoint();
}

void CGlanixState_Rage2StartJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && m_pModelCom->Get_CurrAnimationFrame() <= 75)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2START_TURN_OL);
	}
}

void CGlanixState_Rage2StartJump::Exit_State()
{
}

CGlanixState_Rage2StartJump* CGlanixState_Rage2StartJump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2StartJump* pInstance = new CGlanixState_Rage2StartJump(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2StartJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2StartJump::Free()
{
	__super::Free();
}


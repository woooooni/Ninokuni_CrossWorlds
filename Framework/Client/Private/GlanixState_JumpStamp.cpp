#include "stdafx.h"
#include "GlanixState_JumpStamp.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"


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

}

void CGlanixState_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if(m_pModelCom->Get_CurrAnimationFrame() < 45)
		vDestPos = m_pPlayerTransform->Get_Position();

	if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && m_pModelCom->Get_CurrAnimationFrame() <= 75)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.35f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() == 76)
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
	}


	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
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


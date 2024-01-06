#include "stdafx.h"
#include "GlanixState_IntroFinish.h"

#include "Glanix.h"

#include "Animation.h"

CGlanixState_IntroFinish::CGlanixState_IntroFinish(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_IntroFinish::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_IntroFinish::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_IntroFinish01"));
}

void CGlanixState_IntroFinish::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// ¾Æ ¸ô¶û
	if (m_pModelCom->Get_CurrAnimationFrame() == 5)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pGlanix->Get_OriginPos());
		m_pGlanix->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Set_Velocity({ 0.f, 0.f, 0.f });
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_IntroFinish::Exit_State()
{
}

CGlanixState_IntroFinish* CGlanixState_IntroFinish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_IntroFinish* pInstance = new CGlanixState_IntroFinish(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_IntroFinish");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_IntroFinish::Free()
{
	__super::Free();
}


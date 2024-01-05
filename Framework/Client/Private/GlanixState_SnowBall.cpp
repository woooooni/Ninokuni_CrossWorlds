#include "stdafx.h"
#include "GlanixState_SnowBall.h"

#include "GameInstance.h"

#include "Glanix.h"
#include "Glanix_IceBall.h"

CGlanixState_SnowBall::CGlanixState_SnowBall(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_SnowBall::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_SnowBall::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill08"));
	m_bIsRoll = false;
}

void CGlanixState_SnowBall::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	//if (m_pModelCom->Get_CurrAnimationFrame() == 45 && !m_bIsRoll)
	//{
	//	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_GlanixIceBall"), m_pGlanix);
	//	m_bIsRoll = true;
	//}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
}

void CGlanixState_SnowBall::Exit_State()
{
}

CGlanixState_SnowBall* CGlanixState_SnowBall::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_SnowBall* pInstance = new CGlanixState_SnowBall(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_SnowBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_SnowBall::Free()
{
	__super::Free();
}

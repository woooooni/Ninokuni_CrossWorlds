#include "stdafx.h"
#include "GlanixState_Rage2Rising.h"

#include "Glanix.h"

#include "Animation.h"

CGlanixState_Rage2Rising::CGlanixState_Rage2Rising(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Rising::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fFlyTime = 2.f;

	return S_OK;
}

void CGlanixState_Rage2Rising::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalJump"));

	m_fTime = 0.f;
}

void CGlanixState_Rage2Rising::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//m_fTime += fTimeDelta;
		//if (m_fTime >= m_fFlyTime)
		//{
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2STAMP);
		//}
	}
}

void CGlanixState_Rage2Rising::Exit_State()
{
}

CGlanixState_Rage2Rising* CGlanixState_Rage2Rising::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Rising* pInstance = new CGlanixState_Rage2Rising(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Rising");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Rising::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GlanixState_Dead.h"

#include "Glanix.h"

CGlanixState_Dead::CGlanixState_Dead(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Death"));
}

void CGlanixState_Dead::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pGlanix->Reserve_Dead(true);
	}
}

void CGlanixState_Dead::Exit_State()
{
}

CGlanixState_Dead* CGlanixState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Dead* pInstance = new CGlanixState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Dead::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "Glanix_PhoenixState_Idle.h"

#include "Glanix_Phoenix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CGlanix_PhoenixState_Idle::CGlanix_PhoenixState_Idle(CStateMachine* pStateMachine)
	: CGlanixPhoenixState_Base(pStateMachine)
{
}

HRESULT CGlanix_PhoenixState_Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanix_PhoenixState_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Phoenix.ao|Phoenix_Stand"));
}

void CGlanix_PhoenixState_Idle::Tick_State(_float fTimeDelta)
{
	if (m_pPhoenix->Get_IsOut())
	{
		m_pStateMachineCom->Change_State(CGlanix_Phoenix::PHOENIX_OUT);
	}
}

void CGlanix_PhoenixState_Idle::Exit_State()
{
}

CGlanix_PhoenixState_Idle* CGlanix_PhoenixState_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanix_PhoenixState_Idle* pInstance = new CGlanix_PhoenixState_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanix_PhoenixState_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_PhoenixState_Idle::Free()
{
	__super::Free();
}


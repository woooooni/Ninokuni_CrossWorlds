#include "stdafx.h"
#include "Glanix_PhoenixState_Spawn.h"

#include "Glanix_Phoenix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CGlanix_PhoenixState_Spawn::CGlanix_PhoenixState_Spawn(CStateMachine* pStateMachine)
	: CGlanixPhoenixState_Base(pStateMachine)
{
}

HRESULT CGlanix_PhoenixState_Spawn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanix_PhoenixState_Spawn::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Phoenix.ao|Phoenix_Spawn"));
}

void CGlanix_PhoenixState_Spawn::Tick_State(_float fTimeDelta)
{
	// player가 없으면 그냥 굳어버리게.
	if (m_pPlayer != nullptr)
	{
		if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		{
			m_pStateMachineCom->Change_State(CGlanix_Phoenix::PHOENIX_IDLE);
		}
	}
}

void CGlanix_PhoenixState_Spawn::Exit_State()
{
}

CGlanix_PhoenixState_Spawn* CGlanix_PhoenixState_Spawn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanix_PhoenixState_Spawn* pInstance = new CGlanix_PhoenixState_Spawn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanix_PhoenixState_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_PhoenixState_Spawn::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "Glanix_PhoenixState_Out.h"

#include "Glanix_Phoenix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CGlanix_PhoenixState_Out::CGlanix_PhoenixState_Out(CStateMachine* pStateMachine)
	: CGlanixPhoenixState_Base(pStateMachine)
{
}

HRESULT CGlanix_PhoenixState_Out::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanix_PhoenixState_Out::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Phoenix.ao|Phoenix_Success"));
}

void CGlanix_PhoenixState_Out::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_Phoenix"), &m_pPhoenix->Get_OriginPos());
		m_pPhoenix->Set_Dead(true);
	}
}

void CGlanix_PhoenixState_Out::Exit_State()
{
}

CGlanix_PhoenixState_Out* CGlanix_PhoenixState_Out::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanix_PhoenixState_Out* pInstance = new CGlanix_PhoenixState_Out(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanix_PhoenixState_Out");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_PhoenixState_Out::Free()
{
	__super::Free();
}


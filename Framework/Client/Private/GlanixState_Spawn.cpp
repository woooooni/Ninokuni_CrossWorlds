#include "stdafx.h"
#include "GlanixState_Spawn.h"

#include "Glanix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CGlanixState_Spawn::CGlanixState_Spawn(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Spawn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Spawn::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Spawn_New"));
	// m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao | Glanix_LeftTurn180"));

	_vector vPillarPos = { 8.f, 0.f, -8.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -11.f, 0.f, -8.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { 12.f, 0.f, 11.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -6.f, 0.f, 7.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);

	if(m_pPlayer != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());
}

void CGlanixState_Spawn::Tick_State(_float fTimeDelta)
{
	// player가 없으면 그냥 굳어버리게.
	if (m_pPlayer != nullptr)
	{
		if (m_pModelCom->Get_CurrAnimationFrame() >= 80 && m_pModelCom->Get_CurrAnimationFrame() <= 100 ||
			m_pModelCom->Get_CurrAnimationFrame() == 15)
		{
			CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		}

		if (m_pModelCom->Is_Finish())
		{
			//_float fWaitTime = 1.f;
			//m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_JUMPSTAMP);
		}
	}
}

void CGlanixState_Spawn::Exit_State()
{
}

CGlanixState_Spawn* CGlanixState_Spawn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Spawn* pInstance = new CGlanixState_Spawn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Spawn::Free()
{
	__super::Free();
}


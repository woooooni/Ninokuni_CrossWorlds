#include "stdafx.h"
#include "GlanixState_RageStart.h"

#include "Glanix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CGlanixState_RageStart::CGlanixState_RageStart(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
}

void CGlanixState_RageStart::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimationFrame() >= 75 && m_pModelCom->Get_CurrAnimationFrame() <= 95)
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageStart::Exit_State()
{
	//_float4 vGlanixPos = {};
	//XMStoreFloat4(&vGlanixPos, m_pTransformCom->Get_Position());
	//
	//_vector vPillarPos = { vGlanixPos.x + 8.f, 1.6f, vGlanixPos.z + -8.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	//vPillarPos = { vGlanixPos.x + -15.f, 1.6f, vGlanixPos.z + -8.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	//vPillarPos = { vGlanixPos.x + 12.f, 1.6f, vGlanixPos.z + 11.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	//vPillarPos = { vGlanixPos.x + -11.f, 1.6f, vGlanixPos.z + 13.f, 1.f };
	//GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);

	_vector vPillarPos = { -64.f, 1.6f, 360.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -34.f, 1.6f, 350.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -60.f, 1.6f, 334.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -36.f, 1.6f, 378.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -63.f, 1.6f, 378.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -44.f, 1.6f, 391.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
	vPillarPos = { -23.f, 1.6f, 363.f, 1.f };
	GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPillarPos);
}

CGlanixState_RageStart* CGlanixState_RageStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStart* pInstance = new CGlanixState_RageStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStart::Free()
{
	__super::Free();
}

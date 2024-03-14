#include "stdafx.h"
#include "GlanixState_RageCrash.h"

#include "Glanix.h"
#include "Camera_Manager.h"
#include "Particle_Manager.h"

CGlanixState_RageCrash::CGlanixState_RageCrash(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageCrash::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageCrash::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Crash"));

	CCamera_Manager::GetInstance()->Start_Action_Shake_Default();

	m_bEffectCreate = false;
}

void CGlanixState_RageCrash::Tick_State(_float fTimeDelta)
{
	if (false == m_pModelCom->Is_Tween())
	{
		if (m_pModelCom->Get_CurrAnimationFrame() < 10)
		{
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), -10.f, fTimeDelta);
		}

		else if (false == m_bEffectCreate && m_pModelCom->Get_CurrAnimationFrame() >= 20)
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Atchi_03"),
				m_pTransformCom->Get_WorldMatrix(), _float3(1.f, 2.7f, 2.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

			m_bEffectCreate = true;
		}

		else if (m_pModelCom->Is_Finish())
		{
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
		}
	}
}

void CGlanixState_RageCrash::Exit_State()
{
}

CGlanixState_RageCrash* CGlanixState_RageCrash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageCrash* pInstance = new CGlanixState_RageCrash(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageCrash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageCrash::Free()
{
	__super::Free();
}

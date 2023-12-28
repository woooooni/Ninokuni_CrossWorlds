#include "stdafx.h"
#include "GlanixState_RagePull.h"

#include "Glanix.h"

#include "Animation.h"

CGlanixState_RagePull::CGlanixState_RagePull(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RagePull::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RagePull::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageSkillPull"));
}

void CGlanixState_RagePull::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 20 && m_pModelCom->Get_CurrAnimationFrame() <= 175)
	{
		_vector vPullDir = XMVector3Normalize(m_pTransformCom->Get_Position() - m_pPlayerTransform->Get_Position());
		m_pPlayer->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(vPullDir, 4.5f, true);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGERISING);
	}
}

void CGlanixState_RagePull::Exit_State()
{
}

CGlanixState_RagePull* CGlanixState_RagePull::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RagePull* pInstance = new CGlanixState_RagePull(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RagePull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RagePull::Free()
{
	__super::Free();
}


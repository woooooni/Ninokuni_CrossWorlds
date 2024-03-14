#include "stdafx.h"
#include "GlanixState_SpinBombBomb.h"

#include "Glanix.h"
#include "Camera_Manager.h"
#include "Effect_Manager.h"

CGlanixState_SpinBombBomb::CGlanixState_SpinBombBomb(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_SpinBombBomb::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_SpinBombBomb::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill09"));

	// Effect Create
	CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_SwingDownDown"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
}

void CGlanixState_SpinBombBomb::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
}

void CGlanixState_SpinBombBomb::Exit_State()
{
}

CGlanixState_SpinBombBomb* CGlanixState_SpinBombBomb::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_SpinBombBomb* pInstance = new CGlanixState_SpinBombBomb(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_SpinBombBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_SpinBombBomb::Free()
{
	__super::Free();
}

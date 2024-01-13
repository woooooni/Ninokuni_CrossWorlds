#include "stdafx.h"
#include "GlanixState_Attack1.h"

#include "Glanix.h"
#include "Effect_Manager.h"
#include "Animation.h"

CGlanixState_Attack1::CGlanixState_Attack1(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Attack1::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Attack1::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Attack01"));

	// Effect Create
	CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_TwoHandSwing"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
}

void CGlanixState_Attack1::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	//if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_Attack01"))
	//{
	//	if (m_pModelCom->Get_CurrAnimationFrame() >= 30 && m_pModelCom->Get_CurrAnimationFrame() <= 44)
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
	//	else if (m_pModelCom->Get_CurrAnimationFrame() >= 70 && m_pModelCom->Get_CurrAnimationFrame() <= 88)
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
	//	else if (m_pModelCom->Get_CurrAnimationFrame() == 45 || m_pModelCom->Get_CurrAnimationFrame() == 89)
	//	{
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	//	}
	//}

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_Attack01"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() >= 30 && m_pModelCom->Get_CurrAnimationFrame() <= 89)
			m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
		else if (m_pModelCom->Get_CurrAnimationFrame() >= 90)
			m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
}

void CGlanixState_Attack1::Exit_State()
{
}

CGlanixState_Attack1* CGlanixState_Attack1::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Attack1* pInstance = new CGlanixState_Attack1(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Attack1::Free()
{
	__super::Free();
}


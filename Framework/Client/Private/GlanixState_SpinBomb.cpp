#include "stdafx.h"
#include "GlanixState_SpinBomb.h"

#include "Glanix.h"
#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Animation.h"

CGlanixState_SpinBomb::CGlanixState_SpinBomb(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_SpinBomb::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_SpinBomb::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill01"));

	// Effect Create
	CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_SwingDown"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
	m_pGlanix->Generate_MotionTrail(m_MotionTrailDesc);

}

void CGlanixState_SpinBomb::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	//if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_Skill01"))
	//{
	//	if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 45)
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
	//	else if (m_pModelCom->Get_CurrAnimationFrame() >= 100 && m_pModelCom->Get_CurrAnimationFrame() <= 105)
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
	//	else if (m_pModelCom->Get_CurrAnimationFrame() == 46 || m_pModelCom->Get_CurrAnimationFrame() == 106)
	//	{
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	//	}
	//}

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_Skill01"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() >= 35)
			m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
		else if (m_pModelCom->Get_CurrAnimationFrame() >= 106)
			m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
}

void CGlanixState_SpinBomb::Exit_State()
{
	m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	m_pGlanix->Stop_MotionTrail();
}

CGlanixState_SpinBomb* CGlanixState_SpinBomb::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_SpinBomb* pInstance = new CGlanixState_SpinBomb(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_SpinBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_SpinBomb::Free()
{
	__super::Free();
}


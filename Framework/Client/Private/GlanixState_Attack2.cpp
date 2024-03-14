#include "stdafx.h"
#include "GlanixState_Attack2.h"

#include "Glanix.h"
#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Animation.h"

CGlanixState_Attack2::CGlanixState_Attack2(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Attack2::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Attack2::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Attack02"));

	// Effect Create
	CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_FootDown"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
	m_pGlanix->Generate_MotionTrail(m_MotionTrailDesc);
}

void CGlanixState_Attack2::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	//if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_Attack02"))
	//{
	//	if (m_pModelCom->Get_CurrAnimationFrame() >= 30 && m_pModelCom->Get_CurrAnimationFrame() <= 55)
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
	//	else if(m_pModelCom->Get_CurrAnimationFrame() == 56)
	//		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	//}

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_Attack02"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() >= 15 && m_pModelCom->Get_CurrAnimationFrame() <= 55)
			m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
		else if (m_pModelCom->Get_CurrAnimationFrame() == 56)
			m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
}

void CGlanixState_Attack2::Exit_State()
{
	m_pGlanix->Stop_MotionTrail();
}

CGlanixState_Attack2* CGlanixState_Attack2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Attack2* pInstance = new CGlanixState_Attack2(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Attack2::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GlanixState_Chase.h"

#include "Glanix.h"

CGlanixState_Chase::CGlanixState_Chase(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Chase::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChaseTime = 2.f;

	return S_OK;
}

void CGlanixState_Chase::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Battlerun"));

	m_fTime = 0.f;
}

void CGlanixState_Chase::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fTime += fTimeDelta;

	if (m_fTime >= m_fChaseTime)
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_JUMPSTAMP);
		return;
	}

	if (m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp / 2.f && !m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pGlanix->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_BERSERK);
		return;
	}

	if (m_pGlanix->Get_TargetDesc().pTarget != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position());
	
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);

	// 기본 공격 패턴이면 
	if (m_vecAtkState[m_iAtkIndex] == CGlanix::GLANIX_ATTACK1 || m_vecAtkState[m_iAtkIndex] == CGlanix::GLANIX_ATTACK2)
	{
		if (m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
	}
	// 스킬 패턴이면(사정거리 긴)
	else
	{
		if (m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
	}
}

void CGlanixState_Chase::Exit_State()
{
}

CGlanixState_Chase* CGlanixState_Chase::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Chase* pInstance = new CGlanixState_Chase(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Chase::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "StelliaState_Chase.h"

#include "Stellia.h"

CStelliaState_Chase::CStelliaState_Chase(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Chase::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChaseTime = 2.f;

	return S_OK;
}

void CStelliaState_Chase::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Sprint"));
	
	m_fTime = 0.f;
}

void CStelliaState_Chase::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fTime += fTimeDelta;

	if (m_fTime >= m_fChaseTime)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASEJUMPSTAMP);
		return;
	}

	if (m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp / 2.f && !m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pStellia->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_BERSERK);
		return;
	}

	if (m_pPlayer != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);

	// 기본 공격 패턴이면 
	if (m_vecAtkState[m_iAtkIndex] == CStellia::STELLIA_ATTACK1 || m_vecAtkState[m_iAtkIndex] == CStellia::STELLIA_ATTACK2 ||
		m_vecAtkState[m_iAtkIndex] == CStellia::STELLIA_SPINTAIL)
	{
		if (m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
	}
	// 스킬 패턴이면(사정거리 긴)
	else
	{
		if (m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
	}
}

void CStelliaState_Chase::Exit_State()
{
}

CStelliaState_Chase* CStelliaState_Chase::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Chase* pInstance = new CStelliaState_Chase(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Chase::Free()
{
	__super::Free();
}


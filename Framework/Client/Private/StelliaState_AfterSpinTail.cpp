#include "stdafx.h"
#include "StelliaState_AfterSpinTail.h"

#include "Animation.h"
#include "Stellia.h"

#include "Effect_Manager.h"
#include "Decal.h"

CStelliaState_AfterSpinTail::CStelliaState_AfterSpinTail(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_AfterSpinTail::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_AfterSpinTail::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill01"));
	m_bIsSkipFrame = false;
	//m_fOriginStelliaAnimSpeed = m_pModelCom->Get_Animation("SKM_Stellia.ao|Stellia_BossSkill01")->Get_OriginSpeed();
	//m_pModelCom->Get_Animation("SKM_Stellia.ao|Stellia_BossSkill01")->Set_OriginSpeed(m_fOriginStelliaAnimSpeed * 2.5f);
}

void CStelliaState_AfterSpinTail::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Stellia.ao|Stellia_BossSkill01") &&
		m_pModelCom->Get_CurrAnimationFrame() <= 30 && !m_bIsSkipFrame)
	{
		m_pModelCom->Set_KeyFrame_By_Progress(0.25f);
		m_bIsSkipFrame = true;
	}

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Stellia.ao|Stellia_BossSkill01"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() <= 50)
			m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);

		else
			m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		// m_pStateMachineCom->Change_State(CStellia::STELLIA_BACKSTEP);
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}
}

void CStelliaState_AfterSpinTail::Exit_State()
{
	//m_pModelCom->Get_CurrAnimation()->Set_OriginSpeed(m_fOriginStelliaAnimSpeed);
}

CStelliaState_AfterSpinTail* CStelliaState_AfterSpinTail::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_AfterSpinTail* pInstance = new CStelliaState_AfterSpinTail(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_AfterSpinTail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_AfterSpinTail::Free()
{
	__super::Free();
}


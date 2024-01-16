#include "stdafx.h"
#include "StelliaState_Attack1.h"

#include "Stellia.h"
#include "Animation.h"

CStelliaState_Attack1::CStelliaState_Attack1(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Attack1::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Attack1::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Attack01"));
}

void CStelliaState_Attack1::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Stellia.ao|Stellia_Attack01"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() >= 10 && m_pModelCom->Get_CurrAnimationFrame() <= 22)
			m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);
		else if (m_pModelCom->Get_CurrAnimationFrame() >= 23)
			m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	}


	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}
}

void CStelliaState_Attack1::Exit_State()
{
}

CStelliaState_Attack1* CStelliaState_Attack1::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Attack1* pInstance = new CStelliaState_Attack1(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Attack1::Free()
{
	__super::Free();
}


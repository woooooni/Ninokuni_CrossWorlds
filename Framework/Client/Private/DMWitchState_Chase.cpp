#include "stdafx.h"
#include "DMWitchState_Chase.h"

#include "DMWitch.h"

CDMWitchState_Chase::CDMWitchState_Chase(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Chase::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Chase::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk"));
}

void CDMWitchState_Chase::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pWitch->Get_Stat().fHp <= m_pWitch->Get_Stat().fMaxHp / 2.f && !m_pWitch->Get_IsRage())
	{
		m_pWitch->Set_IsRage(true);
		m_pWitch->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_RAGE);
		return;
	}

	if (m_pPlayer != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);

	if (m_pWitch->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
	{
		if (m_iAtkIndex >= m_vecAtkState.size())
			m_iAtkIndex = 0;

		m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
	}
}

void CDMWitchState_Chase::Exit_State()
{
}

CDMWitchState_Chase* CDMWitchState_Chase::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Chase* pInstance = new CDMWitchState_Chase(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Chase::Free()
{
	__super::Free();
}


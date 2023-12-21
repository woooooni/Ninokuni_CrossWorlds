#include "stdafx.h"
#include "StelliaState_Rage.h"

#include "Stellia.h"

CStelliaState_Rage::CStelliaState_Rage(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));
}

void CStelliaState_Rage::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_Rage::Exit_State()
{
}

CStelliaState_Rage* CStelliaState_Rage::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage* pInstance = new CStelliaState_Rage(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage::Free()
{
	__super::Free();
}


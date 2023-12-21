#include "stdafx.h"
#include "StelliaState_JumpStamp.h"

#include "Stellia.h"

CStelliaState_JumpStamp::CStelliaState_JumpStamp(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_JumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_JumpStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02_New"));
}

void CStelliaState_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_JumpStamp::Exit_State()
{
}

CStelliaState_JumpStamp* CStelliaState_JumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_JumpStamp* pInstance = new CStelliaState_JumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_JumpStamp::Free()
{
	__super::Free();
}


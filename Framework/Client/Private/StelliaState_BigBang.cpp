#include "stdafx.h"
#include "StelliaState_BigBang.h"

#include "Stellia.h"

CStelliaState_BigBang::CStelliaState_BigBang(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_BigBang::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_BigBang::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill04_New"));
}

void CStelliaState_BigBang::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_BigBang::Exit_State()
{
}

CStelliaState_BigBang* CStelliaState_BigBang::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_BigBang* pInstance = new CStelliaState_BigBang(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_BigBang");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_BigBang::Free()
{
	__super::Free();
}


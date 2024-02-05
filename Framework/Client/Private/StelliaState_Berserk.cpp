#include "stdafx.h"
#include "StelliaState_Berserk.h"

#include "Stellia.h"

CStelliaState_Berserk::CStelliaState_Berserk(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Berserk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Berserk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));
}

void CStelliaState_Berserk::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_Berserk::Exit_State()
{
	// TODO Change to Moon

}

CStelliaState_Berserk* CStelliaState_Berserk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Berserk* pInstance = new CStelliaState_Berserk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Berserk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Berserk::Free()
{
	__super::Free();
}


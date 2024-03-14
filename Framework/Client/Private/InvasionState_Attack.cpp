#include "stdafx.h"
#include "InvasionState_Attack.h"

#include "Animation.h"

CInvasionState_Attack::CInvasionState_Attack(CStateMachine* pStateMachine)
	: CInvasionState_Base(pStateMachine)
{
}

HRESULT CInvasionState_Attack::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CInvasionState_Attack::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CInvasionState_Attack::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CInvasionState_Attack::Exit_State()
{
}

CInvasionState_Attack* CInvasionState_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CInvasionState_Attack* pInstance = new CInvasionState_Attack(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CInvasionState_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvasionState_Attack::Free()
{
	__super::Free();
}


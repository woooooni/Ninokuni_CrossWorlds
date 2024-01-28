#include "stdafx.h"
#include "InvasionState_Dead.h"

#include "Animation.h"

CInvasionState_Dead::CInvasionState_Dead(CStateMachine* pStateMachine)
	: CInvasionState_Base(pStateMachine)
{
}

HRESULT CInvasionState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CInvasionState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CInvasionState_Dead::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CInvasionState_Dead::Exit_State()
{
}

CInvasionState_Dead* CInvasionState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CInvasionState_Dead* pInstance = new CInvasionState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CInvasionState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvasionState_Dead::Free()
{
	__super::Free();
}


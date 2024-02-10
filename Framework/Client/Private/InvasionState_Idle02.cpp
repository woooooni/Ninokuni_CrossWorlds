#include "stdafx.h"
#include "InvasionState_Idle02.h"

#include "Animation.h"

CInvasionState_Idle02::CInvasionState_Idle02(CStateMachine* pStateMachine)
	: CInvasionState_Base(pStateMachine)
{
}

HRESULT CInvasionState_Idle02::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CInvasionState_Idle02::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CInvasionState_Idle02::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

}

void CInvasionState_Idle02::Exit_State()
{
}

CInvasionState_Idle02* CInvasionState_Idle02::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CInvasionState_Idle02* pInstance = new CInvasionState_Idle02(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CInvasionState_Idle02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvasionState_Idle02::Free()
{
	__super::Free();
}


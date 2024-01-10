#include "stdafx.h"
#include "UniqueNpcState_Idle.h"

#include "GameNpc.h"

#include "Animation.h"

CUniqueNpcState_Idle::CUniqueNpcState_Idle(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CUniqueNpcState_Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CUniqueNpcState_Idle::Exit_State()
{
}

CUniqueNpcState_Idle* CUniqueNpcState_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Idle* pInstance = new CUniqueNpcState_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Idle::Free()
{
	__super::Free();
}


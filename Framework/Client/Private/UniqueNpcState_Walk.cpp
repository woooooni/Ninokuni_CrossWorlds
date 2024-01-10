#include "stdafx.h"
#include "UniqueNpcState_Walk.h"

#include "GameNpc.h"

#include "Animation.h"

CUniqueNpcState_Walk::CUniqueNpcState_Walk(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Walk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Walk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CUniqueNpcState_Walk::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CUniqueNpcState_Walk::Exit_State()
{
}

CUniqueNpcState_Walk* CUniqueNpcState_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Walk* pInstance = new CUniqueNpcState_Walk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Walk::Free()
{
	__super::Free();
}


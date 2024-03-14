#include "stdafx.h"
#include "UniqueNpcState_Run.h"

#include "GameNpc.h"

#include "Animation.h"

CUniqueNpcState_Run::CUniqueNpcState_Run(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Run::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Run::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CUniqueNpcState_Run::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CUniqueNpcState_Run::Exit_State()
{
}

CUniqueNpcState_Run* CUniqueNpcState_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Run* pInstance = new CUniqueNpcState_Run(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Run::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "UniqueNpcState_Talk.h"

#include "GameNpc.h"

#include "Animation.h"

CUniqueNpcState_Talk::CUniqueNpcState_Talk(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Talk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Talk::Enter_State(void* pArg)
{
}

void CUniqueNpcState_Talk::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		m_pStateMachineCom->Change_State(CGameNpc::NPC_IDLE);
}

void CUniqueNpcState_Talk::Exit_State()
{
}

CUniqueNpcState_Talk* CUniqueNpcState_Talk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Talk* pInstance = new CUniqueNpcState_Talk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Talk::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "..\Public\Manager_StateMachine.h"

#include "GameInstance.h"

#include "Manager_State.h"

CManager_StateMachine::CManager_StateMachine()
{
}

HRESULT CManager_StateMachine::Initialize(void* pArg)
{
	return S_OK;
}

void CManager_StateMachine::Tick(const _float& fTimeDelta)
{
	if (nullptr != m_pCurState)
		m_pCurState->Tick_State(fTimeDelta);
}

void CManager_StateMachine::LateTick(const _float& fTimeDelta)
{
	if (nullptr != m_pCurState)
		m_pCurState->LateTick_State(fTimeDelta);
}

void CManager_StateMachine::Render()
{
	if (nullptr != m_pCurState)
		m_pCurState->Render();
}

HRESULT CManager_StateMachine::Add_State(const _uint eStateTag, CManager_State* pState)
{
	if (nullptr != Find_State(eStateTag))
		return E_FAIL;

	m_pStates.emplace(eStateTag, pState);

	pState->Set_StateTag(eStateTag);

	return S_OK;
}

HRESULT CManager_StateMachine::Change_State(const _uint eStateTag)
{
	CManager_State* pState = Find_State(eStateTag);

	if (nullptr == pState)
		return E_FAIL;

	if (nullptr != m_pCurState)
		m_pCurState->Exit_State();

	m_pCurState = pState;

	m_pCurState->Enter_State();

	return S_OK;
}

CManager_State* CManager_StateMachine::Find_State(const _uint eStateTag)
{
	auto iter = m_pStates.find(eStateTag);

	if (iter == m_pStates.end())
		return nullptr;

	return iter->second;
}

CManager_StateMachine* CManager_StateMachine::Create(void* pArg)
{
	CManager_StateMachine* pInstance = new CManager_StateMachine();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Create : CManager_StateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CManager_StateMachine::Free()
{
	__super::Free();

	for (auto& State : m_pStates)
		Safe_Release(State.second);

	m_pStates.clear();
}

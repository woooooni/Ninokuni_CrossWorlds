#include "stdafx.h"
#include "..\Public\State_Animals.h"
#include "Animals.h"
#include "GameInstance.h"

CState_Animals::CState_Animals(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CState_Animals::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	m_pAnimal = static_cast<CAnimals*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pAnimal)
		return E_FAIL;


	return S_OK;
}

void CState_Animals::Free()
{
	__super::Free();
}

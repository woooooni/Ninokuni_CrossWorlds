#include "stdafx.h"
#include "StelliaState_Dead.h"

#include "Stellia.h"

CStelliaState_Dead::CStelliaState_Dead(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Death"));
}

void CStelliaState_Dead::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStellia->Reserve_Dead(true);
	}
}

void CStelliaState_Dead::Exit_State()
{
}

CStelliaState_Dead* CStelliaState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Dead* pInstance = new CStelliaState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Dead::Free()
{
	__super::Free();
}

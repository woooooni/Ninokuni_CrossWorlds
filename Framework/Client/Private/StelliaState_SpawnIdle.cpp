#include "stdafx.h"
#include "StelliaState_SpawnIdle.h"

#include "Stellia.h"

CStelliaState_SpawnIdle::CStelliaState_SpawnIdle(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpawnIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpawnIdle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand"));
}

void CStelliaState_SpawnIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_SPAWNSTAND);
	}
}

void CStelliaState_SpawnIdle::Exit_State()
{
}

CStelliaState_SpawnIdle* CStelliaState_SpawnIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpawnIdle* pInstance = new CStelliaState_SpawnIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpawnIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpawnIdle::Free()
{
	__super::Free();
}


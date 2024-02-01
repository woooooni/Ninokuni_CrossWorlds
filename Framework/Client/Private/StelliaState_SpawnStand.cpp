#include "stdafx.h"
#include "StelliaState_SpawnStand.h"

#include "Stellia.h"

CStelliaState_SpawnStand::CStelliaState_SpawnStand(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpawnStand::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpawnStand::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand"));
}

void CStelliaState_SpawnStand::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CStelliaState_SpawnStand::Exit_State()
{
}

CStelliaState_SpawnStand* CStelliaState_SpawnStand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpawnStand* pInstance = new CStelliaState_SpawnStand(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpawnStand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpawnStand::Free()
{
	__super::Free();
}


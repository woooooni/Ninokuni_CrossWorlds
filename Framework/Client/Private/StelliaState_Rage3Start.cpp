#include "stdafx.h"
#include "StelliaState_Rage3Start.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3Start::CStelliaState_Rage3Start(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Start::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3Start::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));
}

void CStelliaState_Rage3Start::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE2LOOP);
	}
}

void CStelliaState_Rage3Start::Exit_State()
{
}

CStelliaState_Rage3Start* CStelliaState_Rage3Start::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Start* pInstance = new CStelliaState_Rage3Start(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Start::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "StelliaState_Rage2Finish.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CStelliaState_Rage2Finish::CStelliaState_Rage2Finish(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage2Finish::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage2Finish::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill04_New"));
}

void CStelliaState_Rage2Finish::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimationFrame() == 115 &&
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Hp() > 0)
	{
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Decrease_HP(999);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}
}

void CStelliaState_Rage2Finish::Exit_State()
{
}

CStelliaState_Rage2Finish* CStelliaState_Rage2Finish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage2Finish* pInstance = new CStelliaState_Rage2Finish(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage2Finish");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage2Finish::Free()
{
	__super::Free();
}

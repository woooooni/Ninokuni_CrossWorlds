#include "stdafx.h"
#include "StelliaState_Rage3ClawReady.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3ClawReady::CStelliaState_Rage3ClawReady(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3ClawReady::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3ClawReady::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Start"));
}

void CStelliaState_Rage3ClawReady::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position());

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CLAW_CHARGE);
	}
}

void CStelliaState_Rage3ClawReady::Exit_State()
{
}

CStelliaState_Rage3ClawReady* CStelliaState_Rage3ClawReady::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3ClawReady* pInstance = new CStelliaState_Rage3ClawReady(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3ClawReady");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3ClawReady::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "StelliaState_Rage3ChargeReady.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3ChargeReady::CStelliaState_Rage3ChargeReady(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3ChargeReady::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3ChargeReady::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Start"));
}

void CStelliaState_Rage3ChargeReady::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position());

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CHARGE);
	}
}

void CStelliaState_Rage3ChargeReady::Exit_State()
{
}

CStelliaState_Rage3ChargeReady* CStelliaState_Rage3ChargeReady::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3ChargeReady* pInstance = new CStelliaState_Rage3ChargeReady(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3ChargeReady");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3ChargeReady::Free()
{
	__super::Free();
}

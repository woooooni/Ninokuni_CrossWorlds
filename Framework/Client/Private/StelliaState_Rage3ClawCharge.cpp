#include "stdafx.h"
#include "StelliaState_Rage3ClawCharge.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3ClawCharge::CStelliaState_Rage3ClawCharge(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3ClawCharge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3ClawCharge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Loop"));
}

void CStelliaState_Rage3ClawCharge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 스텔리아를 공격 가능한 거리까지 앞으로 보낸다.
	m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position());
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRage3AroundSpeed, fTimeDelta);

	// 스텔리아가 공격 가능한 거리까지 도달하면 공격한다.
	//Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();
	//if (fabs(vCenterToStellia.Length()) < m_fClawDist)
	//{
	//	m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CLAW);
	//}
	if (m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND))
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CLAW);
	}
}

void CStelliaState_Rage3ClawCharge::Exit_State()
{
}

CStelliaState_Rage3ClawCharge* CStelliaState_Rage3ClawCharge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3ClawCharge* pInstance = new CStelliaState_Rage3ClawCharge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3ClawCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3ClawCharge::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "StelliaState_Rage3ClawEndCharge.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3ClawEndCharge::CStelliaState_Rage3ClawEndCharge(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3ClawEndCharge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fMinChargeLength = 6.f;

	return S_OK;
}

void CStelliaState_Rage3ClawEndCharge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Loop"));

	m_vStartPos = m_pTransformCom->Get_Position();
}

void CStelliaState_Rage3ClawEndCharge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 최소 질주 거리 계산(시작하자마자 브레이크 밟는거 방지)
	Vec4 vCurPos = (Vec4)m_pTransformCom->Get_Position() - m_vStartPos;
	m_fCurChargeLength = fabs(vCurPos.Length());

	// 스텔리아가 다시 AroundDist 까지 도달하면 돈다.
	Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();
	if (fabs(vCenterToStellia.Length()) >= m_fAroundDist - 5.f && m_fCurChargeLength > m_fMinChargeLength)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CLAW_ENDBREAK);
	}
	else
	{
		// 스텔리아를 다시 AroundDist 까지 달린다.
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRage3AroundSpeed, fTimeDelta);
	}
}

void CStelliaState_Rage3ClawEndCharge::Exit_State()
{
}

CStelliaState_Rage3ClawEndCharge* CStelliaState_Rage3ClawEndCharge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3ClawEndCharge* pInstance = new CStelliaState_Rage3ClawEndCharge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3ClawEndCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3ClawEndCharge::Free()
{
	__super::Free();
}

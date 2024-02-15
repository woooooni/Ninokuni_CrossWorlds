#include "stdafx.h"
#include "StelliaState_Turn.h"

#include "Stellia.h"

CStelliaState_Turn::CStelliaState_Turn(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Turn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Turn::Enter_State(void* pArg)
{
	////  폭주 아닐 시 폭주 체크
	//if (m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp / 2.f && !m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	//{
	//	m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
	//	m_pStellia->Set_SkillTree();
	//	m_iAtkIndex = 0;
	//	m_pStateMachineCom->Change_State(CStellia::STELLIA_BERSERK);
	//	return;
	//}
	//
	//// 레이지1 패턴
	//if (!m_bIsRageInit && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.75f)
	//{
	//	m_bIsRageInit = true;
	//	m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_RAGE, true);
	//	m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1START_TURN_OC);
	//	return;
	//}
	//
	//// 레이지2 패턴 (위에게 진짜 조건문)
	//if (!m_bIsRage2Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.40f &&
	//	m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	////if (!m_bIsRage2Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.90f)
	//{
	//	m_bIsRage2Init = true;
	//	m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_RAGE2, true);
	//	m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE2START_TURN_OC);
	//	return;
	//}
	//
	//// 레이지3 패턴 (위에게 진짜 조건문)
	//if (!m_bIsRage3Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.15f &&
	//	m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	if (!m_bIsRage3Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.90f)
	{
		m_bIsRage3Init = true;
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_RAGE3, true);
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3START_FADEOUT);
		//m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3START_FADEIN);
		return;
	}


	/* 일반 턴 시작 */
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Run"));

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* 보스위 look과 플레이어 위치 - 보스 위치로 나온 방향 벡터의 각도를 구함. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	///* 만약 회전모션이 필요할 정도가 아닌 각도라면 3초정도의 idle 상태로 전환.*/
	//if (fAngle < 20.f)
	//{
	//	_float fWaitTime = 3.f;
	//	m_pStateMachineCom->Change_State(CStellia::GLANIX_COMBATIDLE, &fWaitTime);
	//}
	/* 회전이 필요한 각도라면 */
	//else
	{
		/* 보스의 look을 기준으로 타겟이 왼쪽, 오른쪽에 위치하는지를 판별. */
		_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
		_float fCrossProductY = XMVectorGetY(vCrossProduct);

		/* 보스가 바라보는 방향을 기준으로 오른쪽에 위치. */
		if (fCrossProductY > 0.f)
		{
			m_fTurnSpeed = 3.f;
		}
		/* 보스가 바라보는 방향을 기준으로 왼쪽에 위치. */
		else if (fCrossProductY < 0.f)
		{
			m_fTurnSpeed = -3.f;
		}
	}

	m_vDestPos = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position();
}

void CStelliaState_Turn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	//_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* 보스의 look을 기준으로 플레이어가 왼쪽, 오른쪽에 위치하는지를 판별. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* 오른쪽으로 턴 */
	if (m_fTurnSpeed > 0.f)
	{
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
		else
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;
			m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASE);
		}
	}
	/* 왼쪽으로 턴 */
	else if (m_fTurnSpeed < 0.f)
	{
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
		else
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;
			m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASE);
		}
	}

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);
}

void CStelliaState_Turn::Exit_State()
{
}

CStelliaState_Turn* CStelliaState_Turn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Turn* pInstance = new CStelliaState_Turn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Turn::Free()
{
	__super::Free();
}


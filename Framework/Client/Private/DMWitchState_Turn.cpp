#include "stdafx.h"
#include "DMWitchState_Turn.h"

#include "DMWitch.h"

CDMWitchState_Turn::CDMWitchState_Turn(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Turn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Turn::Enter_State(void* pArg)
{
	if (m_pWitch->Get_Stat().fHp <= m_pWitch->Get_Stat().fMaxHp / 2.f && !m_pWitch->Get_IsRage())
	{
		m_pWitch->Set_IsRage(true);
		m_pWitch->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_RAGE);
		return;
	}

	m_fWaitTime = 1.f;

	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand"));
	m_fTime = 0.f;

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* 보스위 look과 플레이어 위치 - 보스 위치로 나온 방향 벡터의 각도를 구함. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* 보스의 look을 기준으로 플레이어가 왼쪽, 오른쪽에 위치하는지를 판별. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* 보스가 바라보는 방향을 기준으로 오른쪽에 위치. */
	if (fCrossProductY > 0.f)
	{
		m_fTurnSpeed = 4.f;
	}
	/* 보스가 바라보는 방향을 기준으로 왼쪽에 위치. */
	else if (fCrossProductY < 0.f)
	{
		m_fTurnSpeed = -4.f;
	}

	m_vDestPos = m_pPlayerTransform->Get_Position();
}

void CDMWitchState_Turn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fTime += fTimeDelta;

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());

	/* 보스의 look을 기준으로 플레이어가 왼쪽, 오른쪽에 위치하는지를 판별. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* 오른쪽으로 턴 */
	if (m_fTurnSpeed > 0.f)
	{
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}
	/* 왼쪽으로 턴 */
	else if (m_fTurnSpeed < 0.f)
	{
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}

	if (m_fTime >= m_fWaitTime)
	{
		if (m_pWitch->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
		else
			m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_CHASE);
	}
}

void CDMWitchState_Turn::Exit_State()
{
}

CDMWitchState_Turn* CDMWitchState_Turn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Turn* pInstance = new CDMWitchState_Turn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Turn::Free()
{
	__super::Free();
}


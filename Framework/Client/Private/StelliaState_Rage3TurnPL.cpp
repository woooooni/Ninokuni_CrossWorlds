#include "stdafx.h"
#include "StelliaState_Rage3TurnPL.h"

#include "Stellia.h"

CStelliaState_Rage3TurnPL::CStelliaState_Rage3TurnPL(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3TurnPL::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_vecRage3AtkState.push_back(CStellia::STELLIA_RAGE3CLAW_READY);
	m_vecRage3AtkState.push_back(CStellia::STELLIA_RAGE3CHARGE_READY);

	return S_OK;
}

void CStelliaState_Rage3TurnPL::Enter_State(void* pArg)
{
	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	Vec4 vDest = m_pStellia->Get_OriginPos();
	vDest -= m_pTransformCom->Get_Position();
	_vector vDestNormal = XMVector3Normalize(vDest);

	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* 보스의 look을 기준으로 플레이어가 왼쪽, 오른쪽에 위치하는지를 판별. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* 보스가 바라보는 방향을 기준으로 오른쪽에 위치. */
	if (fCrossProductY > 0.f)
	{
		if (fAngle >= 0.f && fAngle < 90.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_RightTurn"));
			m_fTurnSpeed = 2.f;
		}
		else if (fAngle >= 90.f && fAngle < 180.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_RightTurn180"));
			m_fTurnSpeed = 4.f;
		}
	}
	/* 보스가 바라보는 방향을 기준으로 왼쪽에 위치. */
	else if (fCrossProductY < 0.f)
	{

		if (fAngle >= 0.f && fAngle < 90.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_LeftTurn"));
			m_fTurnSpeed = -2.f;
		}
		else if (fAngle >= 90.f && fAngle < 180.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_InstantTurn"));
			m_fTurnSpeed = -4.f;
		}
	}

	m_vDestPos = m_pStellia->Get_OriginPos();
}

void CStelliaState_Rage3TurnPL::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());

	Vec4 vPosition = m_pTransformCom->Get_Position();
	_vector vDestNormal = XMVector3Normalize(m_vDestPos - vPosition);

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

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(m_vecRage3AtkState[m_iRageAtkIndex++]);
	}
}

void CStelliaState_Rage3TurnPL::Exit_State()
{
	if (m_iRageAtkIndex >= m_vecRage3AtkState.size())
		m_iRageAtkIndex = 0;
}

CStelliaState_Rage3TurnPL* CStelliaState_Rage3TurnPL::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3TurnPL* pInstance = new CStelliaState_Rage3TurnPL(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3TurnPL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3TurnPL::Free()
{
	__super::Free();
}


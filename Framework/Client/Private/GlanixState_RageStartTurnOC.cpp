#include "stdafx.h"
#include "GlanixState_RageStartTurnOC.h"

#include "Glanix.h"

CGlanixState_RageStartTurnOC::CGlanixState_RageStartTurnOC(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStartTurnOC::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStartTurnOC::Enter_State(void* pArg)
{
	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	Vec4 vDest = m_pGlanix->Get_OriginPos();
	vDest -= m_pTransformCom->Get_Position();
	_vector vDestNormal = XMVector3Normalize(vDest);

	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* 보스의 look을 기준으로 센터가 왼쪽, 오른쪽에 위치하는지를 판별. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* 보스가 바라보는 방향을 기준으로 오른쪽에 위치. */
	if (fCrossProductY > 0.f)
	{
		if (fAngle >= 0.f && fAngle < 90.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RightTurn"));
			m_fTurnSpeed = 2.f;
		}
		else if (fAngle >= 90.f && fAngle < 180.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RightTurn180"));
			m_fTurnSpeed = 4.f;
		}
	}
	/* 보스가 바라보는 방향을 기준으로 왼쪽에 위치. */
	else if (fCrossProductY < 0.f)
	{

		if (fAngle >= 0.f && fAngle < 90.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_LeftTurn"));
			m_fTurnSpeed = -2.f;
		}
		else if (fAngle >= 90.f && fAngle < 180.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_LeftTurn180"));
			m_fTurnSpeed = -4.f;
		}
	}

	m_vDestPos = m_pGlanix->Get_OriginPos();
}

void CGlanixState_RageStartTurnOC::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());

	Vec4 vPosition = m_pTransformCom->Get_Position();
	_vector vDestNormal = XMVector3Normalize(m_vDestPos - vPosition);

	/* 보스의 look을 기준으로 센터가 왼쪽, 오른쪽에 위치하는지를 판별. */
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
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGESTART_JUMP);
	}
}

void CGlanixState_RageStartTurnOC::Exit_State()
{
}

CGlanixState_RageStartTurnOC* CGlanixState_RageStartTurnOC::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStartTurnOC* pInstance = new CGlanixState_RageStartTurnOC(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStartTurnOC");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStartTurnOC::Free()
{
	__super::Free();
}


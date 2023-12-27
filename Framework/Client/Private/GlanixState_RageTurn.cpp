#include "stdafx.h"
#include "GlanixState_RageTurn.h"

#include "Glanix.h"

CGlanixState_RageTurn::CGlanixState_RageTurn(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageTurn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageTurn::Enter_State(void* pArg)
{
	if (m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp / 2.f && !m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pGlanix->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_BERSERK);
		return;
	}

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* 보스위 look과 플레이어 위치 - 보스 위치로 나온 방향 벡터의 각도를 구함. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* 만약 회전모션이 필요할 정도가 아닌 각도라면 1초정도의 idle 상태로 전환.*/
	if (fAngle < 20.f)
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	}
	/* 회전이 필요한 각도라면 */
	else
	{
		/* 보스의 look을 기준으로 플레이어가 왼쪽, 오른쪽에 위치하는지를 판별. */
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
	}

	m_vDestPos = m_pPlayerTransform->Get_Position();
}

void CGlanixState_RageTurn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

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

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 1.f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGEIDLE, &fWaitTime);
	}
}

void CGlanixState_RageTurn::Exit_State()
{
}

CGlanixState_RageTurn* CGlanixState_RageTurn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageTurn* pInstance = new CGlanixState_RageTurn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageTurn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageTurn::Free()
{
	__super::Free();
}


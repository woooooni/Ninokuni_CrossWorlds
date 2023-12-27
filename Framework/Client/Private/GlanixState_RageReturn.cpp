#include "stdafx.h"
#include "GlanixState_RageReturn.h"

#include "Glanix.h"

#include "Animation.h"

CGlanixState_RageReturn::CGlanixState_RageReturn(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageReturn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageReturn::Enter_State(void* pArg)
{
	// 매개로 처음 진입인지 중간 진입인지 받아서 처리하자.
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill03"));

	vDestPos = m_pGlanix->Get_OriginPos();
}

void CGlanixState_RageReturn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && m_pModelCom->Get_CurrAnimationFrame() <= 75)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}


	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_RageReturn::Exit_State()
{
}

CGlanixState_RageReturn* CGlanixState_RageReturn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageReturn* pInstance = new CGlanixState_RageReturn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageReturn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageReturn::Free()
{
	__super::Free();
}


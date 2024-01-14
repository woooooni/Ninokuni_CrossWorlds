#include "stdafx.h"
#include "GlanixState_RageOffsetJump.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "Camera_Follow.h"
#include "Effect_Manager.h"


CGlanixState_RageOffsetJump::CGlanixState_RageOffsetJump(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageOffsetJump::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageOffsetJump::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill05"));

	// Effect Create
	// CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
	// if (pTransformCom == nullptr)
	// 	return;
	// GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_JumpDown"), pTransformCom->Get_WorldMatrix(), m_pGlanix);

	vDestPos = m_pGlanix->Get_OriginPos();
}

void CGlanixState_RageOffsetJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 15 && m_pModelCom->Get_CurrAnimationFrame() <= 35)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), 12.f, fTimeDelta);
		// XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta * 20.f);
		// m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_float fWaitTime = 2.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageOffsetJump::Exit_State()
{
}

CGlanixState_RageOffsetJump* CGlanixState_RageOffsetJump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageOffsetJump* pInstance = new CGlanixState_RageOffsetJump(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageOffsetJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageOffsetJump::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GlanixState_RageStartJump.h"

#include "Glanix.h"

#include "Animation.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"

CGlanixState_RageStartJump::CGlanixState_RageStartJump(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStartJump::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStartJump::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill03"));

	vDestPos = m_pGlanix->Get_OriginPos();

	m_bJumpEffectCreate = false;
	m_bDownEffectCreate = false;
}

void CGlanixState_RageStartJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* Camera */
	if (45 == m_pModelCom->Get_CurrAnimationFrame())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}

		// Effect Create
		if (false == m_bJumpEffectCreate)
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_JumpDown_Smoke"),
				m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.7f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Circle"),
				m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

			m_bJumpEffectCreate = true;
		}
	}

	else if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && m_pModelCom->Get_CurrAnimationFrame() <= 75)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.25f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);

		if (false == m_bDownEffectCreate && 75 == m_pModelCom->Get_CurrAnimationFrame())
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_RageStamp"), m_pTransformCom->Get_WorldMatrix(), m_pGlanix);
			m_bDownEffectCreate = true;
		}
	}

	else if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGESTART_TURN_OL);
	}
}

void CGlanixState_RageStartJump::Exit_State()
{
}

CGlanixState_RageStartJump* CGlanixState_RageStartJump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStartJump* pInstance = new CGlanixState_RageStartJump(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStartJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStartJump::Free()
{
	__super::Free();
}


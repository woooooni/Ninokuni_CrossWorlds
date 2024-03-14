#include "stdafx.h"
#include "StelliaState_Rage1StartJump.h"

#include "Stellia.h"

#include "Animation.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

#include "Particle_Manager.h"

CStelliaState_Rage1StartJump::CStelliaState_Rage1StartJump(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1StartJump::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1StartJump::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02_New"));

	vDestPos = m_pStellia->Get_OriginPos();
	m_bJumpEffect = false;
}

void CStelliaState_Rage1StartJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* Camera Height Limit */
	if (32 == m_pModelCom->Get_CurrAnimationFrame() && !m_pModelCom->Is_Tween())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && pFollowCam->Is_LockOn() && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}
	}

	if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 90)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (!m_bJumpEffect && !m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() >= 91)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Smoke"),
			m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.7f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Circle"),
			m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Circle_02"),
			m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

		m_bJumpEffect = true;
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1START_TURN_OL);
	}
}

void CStelliaState_Rage1StartJump::Exit_State()
{
}

CStelliaState_Rage1StartJump* CStelliaState_Rage1StartJump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1StartJump* pInstance = new CStelliaState_Rage1StartJump(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1StartJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1StartJump::Free()
{
	__super::Free();
}


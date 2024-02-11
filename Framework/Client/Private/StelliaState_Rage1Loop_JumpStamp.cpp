#include "stdafx.h"
#include "StelliaState_Rage1Loop_JumpStamp.h"

#include "Animation.h"
#include "Stellia.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "Decal.h"

#include "Camera_Group.h"

CStelliaState_Rage1Loop_JumpStamp::CStelliaState_Rage1Loop_JumpStamp(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Loop_JumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1Loop_JumpStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02"));

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage01JumpStamp"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Rage1Loop_JumpStamp::Tick_State(_float fTimeDelta)
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

	if (m_pModelCom->Get_CurrAnimationFrame() < 25)
		vDestPos = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position();

	else if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 60)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.35f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	else if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_TURN);
	}
}

void CStelliaState_Rage1Loop_JumpStamp::Exit_State()
{
}

CStelliaState_Rage1Loop_JumpStamp* CStelliaState_Rage1Loop_JumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Loop_JumpStamp* pInstance = new CStelliaState_Rage1Loop_JumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Loop_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Loop_JumpStamp::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "StelliaState_Rage1Loop_JumpStamp.h"

#include "Animation.h"
#include "Stellia.h"

#include "Effect_Manager.h"
#include "Decal.h"

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
}

void CStelliaState_Rage1Loop_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pDecal == nullptr)
	{
		CEffect_Manager::GetInstance()->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"), m_pTransformCom->Get_WorldMatrix(),
			Vec3(0.f, 0.f, 0.f), Vec3(12.f, 5.f, 12.f), Vec3(0.f, 0.f, 0.f), m_pPlayer, &m_pDecal, false);
		Safe_AddRef(m_pDecal);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() < 25)
		vDestPos = m_pPlayerTransform->Get_Position();
	else
		m_pDecal->Set_Owner(nullptr);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 60)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.35f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() == 60)
	{
		if (m_pDecal != nullptr)
		{
			m_pDecal->Set_Dead(true);
			Safe_Release(m_pDecal);
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
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


#include "stdafx.h"
#include "StelliaState_JumpStamp.h"

#include "Animation.h"
#include "Stellia.h"

#include "Effect_Manager.h"
#include "Decal.h"

CStelliaState_JumpStamp::CStelliaState_JumpStamp(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_JumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_JumpStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02_New"));

	// Effect Create
	CTransform* pTransformCom = m_pStellia->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_JumpStamp"), pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pDecal == nullptr && m_pModelCom->Get_CurrAnimationFrame() <= 70)
	{
		CEffect_Manager::GetInstance()->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"), m_pTransformCom->Get_WorldMatrix(),
			Vec3(0.f, 0.f, 0.f), Vec3(12.f, 2.f, 12.f), Vec3(0.f, 0.f, 0.f), m_pPlayer, &m_pDecal, false);
		Safe_AddRef(m_pDecal);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() < 35)
		vDestPos = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position();
	else
	{
		if (m_pDecal != nullptr)
			m_pDecal->Set_Owner(nullptr);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 80)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		Vec4 vFinalPosition = vDestPos;
		Vec3 vReverseDir = XMVector3Normalize(m_pTransformCom->Get_Position() - vDestPos);
		vFinalPosition = vFinalPosition + vReverseDir * 3.f;

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vFinalPosition, fTimeDelta / 0.35f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() == 80)
	{
		if (m_pDecal != nullptr)
		{
			m_pDecal->Set_Dead(true);
			Safe_Release(m_pDecal);
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
		//m_pStateMachineCom->Change_State(CStellia::STELLIA_JUMPSTAMP);
	}

	/* Camera */
	//if (35 == m_pModelCom->Get_CurrAnimationFrame())
	//{
	//	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	//	if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
	//	{
	//		pFollowCam->Lock_LookHeight();
	//	}
	//}

}

void CStelliaState_JumpStamp::Exit_State()
{
	if (m_pDecal != nullptr)
	{
		m_pDecal->Set_Dead(true);
		Safe_Release(m_pDecal);
	}
}

CStelliaState_JumpStamp* CStelliaState_JumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_JumpStamp* pInstance = new CStelliaState_JumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_JumpStamp::Free()
{
	__super::Free();
}


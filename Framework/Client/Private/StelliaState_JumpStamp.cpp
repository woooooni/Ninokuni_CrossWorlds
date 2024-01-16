#include "stdafx.h"
#include "StelliaState_JumpStamp.h"

#include "Animation.h"
#include "Stellia.h"

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
}

void CStelliaState_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() < 35)
		vDestPos = m_pPlayerTransform->Get_Position();

	if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 80)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		Vec4 vFinalPosition = vDestPos;
		Vec3 vReverseDir = XMVector3Normalize(m_pTransformCom->Get_Position() - vDestPos);
		vFinalPosition = vFinalPosition + vReverseDir * 3.f;

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vFinalPosition, fTimeDelta / 0.35f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}


	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
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


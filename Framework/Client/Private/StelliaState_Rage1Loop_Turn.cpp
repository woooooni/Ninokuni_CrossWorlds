#include "stdafx.h"
#include "StelliaState_Rage1Loop_Turn.h"

#include "Stellia.h"

#include "GameInstance.h"

CStelliaState_Rage1Loop_Turn::CStelliaState_Rage1Loop_Turn(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Loop_Turn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1Loop_Turn::Enter_State(void* pArg)
{
	_float fAngle = GI->RandomFloat(90.f, 270.f);

	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_InstantTurn"));
	m_fTurnSpeed = -4.f;

	m_vDestLook = XMVector3Rotate(m_pTransformCom->Get_Look(), XMQuaternionRotationRollPitchYaw(0.0f, fAngle, 0.0f));

	m_pStellia->Set_StelliaHit(false);
}

void CStelliaState_Rage1Loop_Turn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	__super::Rage1_Tick(fTimeDelta);

	// 실시간으로 스텔리아의 룩과 Dest룩을 내적.
	_float fDotProduct = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_Look(), m_vDestLook));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct)); // 우리가 쓸 수 있는 단위로 치환.

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());

	if (fabs(fAngle) > 5.f)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		CStellia::STELLIA_STATE eNextState = CStellia::STELLIA_RAGE1LOOP_EXPLOSION;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_IDLE, &eNextState);
	}
}

void CStelliaState_Rage1Loop_Turn::Exit_State()
{
}

CStelliaState_Rage1Loop_Turn* CStelliaState_Rage1Loop_Turn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Loop_Turn* pInstance = new CStelliaState_Rage1Loop_Turn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Loop_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Loop_Turn::Free()
{
	__super::Free();
}


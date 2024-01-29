#include "stdafx.h"
#include "StelliaState_Rage3TurnAround.h"

#include "Stellia.h"

CStelliaState_Rage3TurnAround::CStelliaState_Rage3TurnAround(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3TurnAround::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fTurnSpeed = 2.f;

	return S_OK;
}

void CStelliaState_Rage3TurnAround::Enter_State(void* pArg)
{
	if (pArg != nullptr)
	{
		m_bIsRight = true;
		m_vLeftRotLook = XMVector3Rotate(m_pTransformCom->Get_Look(), XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(90.f), 0.0f));
		m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_RightTurn"));
	}
	else
	{
		m_bIsRight = false;
		m_vLeftRotLook = XMVector3Rotate(m_pTransformCom->Get_Look(), XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(-90.f), 0.0f));
		m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_LeftTurn"));
	}


}

void CStelliaState_Rage3TurnAround::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vCrossProduct = XMVector3Cross(XMVector3Normalize(m_pTransformCom->Get_Look()), XMVector3Normalize(m_vLeftRotLook));
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	if (!m_bIsRight)
	{
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_fTurnSpeed, fTimeDelta);
	}
	else
	{
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3AROUND);
	}
}

void CStelliaState_Rage3TurnAround::Exit_State()
{
	if (m_iRageAtkIndex >= m_vecRage3AtkState.size())
		m_iRageAtkIndex = 0;
}

CStelliaState_Rage3TurnAround* CStelliaState_Rage3TurnAround::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3TurnAround* pInstance = new CStelliaState_Rage3TurnAround(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3TurnAround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3TurnAround::Free()
{
	__super::Free();
}


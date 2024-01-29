#include "stdafx.h"
#include "StelliaState_BackStep.h"

#include "Stellia.h"

#include "Animation.h"

CStelliaState_BackStep::CStelliaState_BackStep(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_BackStep::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_BackStep::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_InstantTurn"));
	m_fOriginStelliaAnimSpeed = m_pModelCom->Get_Animation("SKM_Stellia.ao|Stellia_InstantTurn")->Get_OriginSpeed();
	m_pModelCom->Get_Animation("SKM_Stellia.ao|Stellia_InstantTurn")->Set_OriginSpeed(m_fOriginStelliaAnimSpeed * 1.5f);

	m_vDestLook = m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position();
}

void CStelliaState_BackStep::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	Vec3 vLook = XMVectorLerp(m_pTransformCom->Get_Look(), XMVector3Normalize(m_vDestLook), fTimeDelta / 1.5f);
	Vec3 vRight = {};
	Vec3 vUp = Vec3::Up;

	vLook.Normalize();

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	vRight = vUp.Cross(vLook);
	vUp = vLook.Cross(vRight);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);

	m_pTransformCom->Move(-m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}
}

void CStelliaState_BackStep::Exit_State()
{
	m_pModelCom->Get_CurrAnimation()->Set_OriginSpeed(m_fOriginStelliaAnimSpeed);
}

CStelliaState_BackStep* CStelliaState_BackStep::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_BackStep* pInstance = new CStelliaState_BackStep(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_BackStep");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_BackStep::Free()
{
	__super::Free();
}


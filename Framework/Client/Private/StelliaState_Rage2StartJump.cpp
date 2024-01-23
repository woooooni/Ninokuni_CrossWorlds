#include "stdafx.h"
#include "StelliaState_Rage2StartJump.h"

#include "Stellia.h"

#include "Animation.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

CStelliaState_Rage2StartJump::CStelliaState_Rage2StartJump(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage2StartJump::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage2StartJump::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02_New"));

	vDestPos = m_pStellia->Get_OriginPos();
}

void CStelliaState_Rage2StartJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 90)
	{
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vDestPos, fTimeDelta / 0.5f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE2START_TURN_OL);
	}
}

void CStelliaState_Rage2StartJump::Exit_State()
{
}

CStelliaState_Rage2StartJump* CStelliaState_Rage2StartJump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage2StartJump* pInstance = new CStelliaState_Rage2StartJump(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage2StartJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage2StartJump::Free()
{
	__super::Free();
}


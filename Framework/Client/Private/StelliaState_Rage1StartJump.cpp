#include "stdafx.h"
#include "StelliaState_Rage1StartJump.h"

#include "Stellia.h"

#include "Animation.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

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
}

void CStelliaState_Rage1StartJump::Tick_State(_float fTimeDelta)
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
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1START_TURN_OL);
	}

	/* Camera */
	if (35 == m_pModelCom->Get_CurrAnimationFrame())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}
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


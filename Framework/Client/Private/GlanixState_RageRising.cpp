#include "stdafx.h"
#include "GlanixState_RageRising.h"

#include "Glanix.h"

#include "Animation.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

CGlanixState_RageRising::CGlanixState_RageRising(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageRising::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fFlyTime = 1.f;

	return S_OK;
}

void CGlanixState_RageRising::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalJump"));

	m_fTime = 0.f;
}

void CGlanixState_RageRising::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		//m_fTime += fTimeDelta;
		//if (m_fTime >= m_fFlyTime)
		//{
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGESTAMP);
		//}
	}

	/* Camera */
	if (20 == m_pModelCom->Get_CurrAnimationFrame())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}
	}
}

void CGlanixState_RageRising::Exit_State()
{
}

CGlanixState_RageRising* CGlanixState_RageRising::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageRising* pInstance = new CGlanixState_RageRising(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageRising");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageRising::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GlanixState_RageStamp.h"

#include "Glanix.h"
#include "GameInstance.h"

#include "Animation.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

#include "Glanix_IcePillar_Controller.h"

CGlanixState_RageStamp::CGlanixState_RageStamp(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalStamp"));
}

void CGlanixState_RageStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 15)
	{
		/* Camera - 팔로우 카메라 전환 */
		const CAMERA_TYPE eCamType = CAMERA_TYPE::FOLLOW;
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(eCamType));
		if (nullptr != pFollowCam && eCamType != CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
		{
			pFollowCam->Set_Default_Position();

			/* 너무 느리게 하면 룩앳 위치 많이 달라짐 */
			CCamera_Manager::GetInstance()->Change_Camera(pFollowCam->Get_Key(), 0.5f, LERP_MODE::SMOOTHER_STEP);

			// 플레이어 공격 인풋 열기
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Attack_Input(true);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Skill_Input(true);
		}


		/* 즉사 여부 결정 */
		{
			const Vec4 vOrigin = m_pGlanix->Get_OriginPos();

			const Vec4 vPlayerPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

			const _float fDist = Vec4(vPlayerPos - vOrigin).xyz().Length();

			if (fDist <= m_pGlanix->Get_PillarsController()->Get_DeathDistnace())
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Decrease_HP(999);
		}
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_STUN);
	}
}

void CGlanixState_RageStamp::Exit_State()
{
}

CGlanixState_RageStamp* CGlanixState_RageStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStamp* pInstance = new CGlanixState_RageStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStamp::Free()
{
	__super::Free();
}


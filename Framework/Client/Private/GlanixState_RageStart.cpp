#include "stdafx.h"
#include "GlanixState_RageStart.h"

#include "Glanix.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Effect_Manager.h"

CGlanixState_RageStart::CGlanixState_RageStart(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStart::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStart::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));

	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Intro_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pGlanix);
}

void CGlanixState_RageStart::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGETURN);
	}
}

void CGlanixState_RageStart::Exit_State()
{
	/* ¾óÀ½ ±âµÕ »ý¼º */
	m_pGlanix->Create_Pillars();

	/* Camera - ÆÈ·Î¿ì -> Å¾ºä */
	CCamera_Top* pCamTop = dynamic_cast<CCamera_Top*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::TOP));
	if (nullptr != pCamTop)
	{
		if (S_OK == pCamTop->Start_TopView(CCamera_Top::VIEW_TYPE::GLANIX_PILLAR_PATTERN))
		{
			CCamera_Manager::GetInstance()->Change_Camera(pCamTop->Get_Key(), 1.25f, LERP_MODE::SMOOTHER_STEP);

			// ÇÃ·¹ÀÌ¾î ÀÎÇ² ¸·±â
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
		}
	}
}

CGlanixState_RageStart* CGlanixState_RageStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStart* pInstance = new CGlanixState_RageStart(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStart::Free()
{
	__super::Free();
}

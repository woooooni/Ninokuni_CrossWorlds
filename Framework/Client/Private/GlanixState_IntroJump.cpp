#include "stdafx.h"
#include "GlanixState_IntroJump.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "Camera_CutScene_Boss.h"
#include "Effect_Manager.h"

CGlanixState_IntroJump::CGlanixState_IntroJump(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_IntroJump::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_IntroJump::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_IntroJump"));
	// m_pTransformCom->LookAt_ForLandObject(m_pGlanix->Get_OriginPos());

	/* Camera */
	{
		CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
		if (nullptr != pCutSceneCam)
			pCutSceneCam->Send_Signal();
	}

	// Effect Create
	{
		CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
		if (pTransformCom == nullptr)
			return;
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Intro_Jump"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
	}
}

void CGlanixState_IntroJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() >= 50 && 
		m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_IntroJump"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() == 50)
		{
			Vec4 vDir = m_pTransformCom->Get_RelativeOffset(Vec4{ 0.f, 1.f, 10.f, 1.f });
			vDir.w = 0.f;
			vDir.Normalize();
			m_pRigidBodyCom->Add_Velocity(vDir.ZeroW().Normalized(), 25.f, true);
		} 

		m_fTime += fTimeDelta;

		if (m_fTime >= 0.3f)
		{
			m_fTime = 0.f;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_INTRO_FINISH);
		}
	}

}

void CGlanixState_IntroJump::Exit_State()
{
}

CGlanixState_IntroJump* CGlanixState_IntroJump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_IntroJump* pInstance = new CGlanixState_IntroJump(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_IntroJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_IntroJump::Free()
{
	__super::Free();
}


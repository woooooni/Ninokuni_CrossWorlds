#include "stdafx.h"
#include "GlanixState_IntroJump.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "Camera_CutScene_Boss.h"

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
}

void CGlanixState_IntroJump::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);


	if (m_pModelCom->Get_CurrAnimationFrame() >= 50 && 
		m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Glanix.ao|Glanix_IntroJump"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() == 50)
		{
			m_pGlanix->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
				{0.f, 1.f, 0.f}, 7.5f, false);
		} 

		m_fTime += fTimeDelta;

		/* y 보존하기 위함. */
		_float4 vCurGlanixPos = {};
		XMStoreFloat4(&vCurGlanixPos, m_pTransformCom->Get_Position());
		_float fY = vCurGlanixPos.y;

		/* 위치 보간. */
		_float4 vCurPos = {};
		XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), m_pGlanix->Get_OriginPos(), fTimeDelta / 0.5f);
		XMStoreFloat4(&vCurPos, vCurVector);
		vCurPos.y = fY;
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vCurPos));

		if (m_fTime >= 2.f)
		{
			m_pGlanix->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
				{ 0.f, 1.f, 0.f }, -1.5f, false);
		}

		if (m_fTime >= 2.5f)
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


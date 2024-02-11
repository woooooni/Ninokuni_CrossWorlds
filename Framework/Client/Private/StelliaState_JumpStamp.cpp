#include "stdafx.h"
#include "StelliaState_JumpStamp.h"

#include "Animation.h"
#include "Stellia.h"

#include "Effect_Manager.h"
#include "Decal.h"

#include "Camera_Group.h"

CStelliaState_JumpStamp::CStelliaState_JumpStamp(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_JumpStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_JumpStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02_New"));
	m_bIsSetY = false;
	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_JumpStamp"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_JumpStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* Camera Height Limit */
	if (32 == m_pModelCom->Get_CurrAnimationFrame() && !m_pModelCom->Is_Tween())
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
		if (nullptr != pFollowCam && pFollowCam->Is_LockOn() && !pFollowCam->Is_Lock_LookHeight())
		{
			pFollowCam->Lock_LookHeight();
		}
	}

	// 공중에 고정시키기 위함(콜라이더 위로 올리기 위함)
	if (m_pModelCom->Get_CurrAnimationFrame() == 35)
	{
		if (!m_bIsSetY)
		{
			Vec4 vStelliaPos = m_pTransformCom->Get_Position();
			m_pStellia->Get_Component_Rigidbody()->Set_Use_Gravity(false);
			vStelliaPos.y = 50.f;
			m_pTransformCom->Set_Position(vStelliaPos);
			m_pStellia->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
			m_bIsSetY = true;
		}
	}

	// 다시 내려오기 위함(콜라이더 원상복귀)
	if (m_pModelCom->Get_CurrAnimationFrame() == 80)
	{
		if (m_bIsSetY)
		{
			Vec4 vStelliaPos = m_pTransformCom->Get_Position();
			m_pStellia->Get_Component_Rigidbody()->Set_Use_Gravity(true);
			vStelliaPos.y = 0.f;
			m_pTransformCom->Set_Position(vStelliaPos);
			m_pStellia->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
			m_bIsSetY = false;
		}
	}

	if (m_pModelCom->Get_CurrAnimationFrame() == 50)
	{
		vDestPos = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position();
		m_pTransformCom->LookAt_ForLandObject(vDestPos);

		Vec4 vFinalPosition = vDestPos;
		Vec3 vReverseDir = XMVector3Normalize(m_pTransformCom->Get_Position() - vDestPos);
		vFinalPosition = vFinalPosition + vReverseDir * 3.f;

		// XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vFinalPosition, fTimeDelta / 0.35f);
		m_pTransformCom->Set_Position(vFinalPosition);
		m_pStellia->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
	
		int i = 0;
	}

	//if (m_pModelCom->Get_CurrAnimationFrame() >= 35 && m_pModelCom->Get_CurrAnimationFrame() <= 80)
	//{
	//	m_pTransformCom->LookAt_ForLandObject(vDestPos);
	//
	//	Vec4 vFinalPosition = vDestPos;
	//	Vec3 vReverseDir = XMVector3Normalize(m_pTransformCom->Get_Position() - vDestPos);
	//	vFinalPosition = vFinalPosition + vReverseDir * 3.f;
	//
	//	XMVECTOR vCurVector = XMVectorLerp(m_pTransformCom->Get_Position(), vFinalPosition, fTimeDelta / 0.35f);
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurVector);
	//}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
		//m_pStateMachineCom->Change_State(CStellia::STELLIA_JUMPSTAMP);
	}
}

void CStelliaState_JumpStamp::Exit_State()
{
}

CStelliaState_JumpStamp* CStelliaState_JumpStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_JumpStamp* pInstance = new CStelliaState_JumpStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_JumpStamp::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Small_Run.h"

CState_SwordMan_Neutral_Pick_Small_Run::CState_SwordMan_Neutral_Pick_Small_Run(CStateMachine* pMachine)
	: CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Small_Run::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;


	return S_OK;
}

void CState_SwordMan_Neutral_Pick_Small_Run::Enter_State(void* pArg)
{
	m_pCharacter->Disappear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Small_Run::Tick_State(_float fTimeDelta)
{
	Input(fTimeDelta);
}

void CState_SwordMan_Neutral_Pick_Small_Run::Exit_State()
{

}

void CState_SwordMan_Neutral_Pick_Small_Run::Input(_float fTimeDelta)
{
	_bool bMove = false;
	if (KEY_HOLD(KEY::W))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * m_fMoveSpeed * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
	}


	if (KEY_HOLD(KEY::S))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * m_fMoveSpeed * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * m_fMoveSpeed * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

		bMove = true;

	}


	if (KEY_HOLD(KEY::D))
	{
		// m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 10.f, -1.f * fTimeDelta);

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * m_fMoveSpeed * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

		bMove = true;
	}



	if (KEY_NONE(KEY::SHIFT))
		m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_WALK);

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE);
	}
}

CState_SwordMan_Neutral_Pick_Small_Run* CState_SwordMan_Neutral_Pick_Small_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_SwordMan_Neutral_Pick_Small_Run* pInstance = new CState_SwordMan_Neutral_Pick_Small_Run(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Small_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SwordMan_Neutral_Pick_Small_Run::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character.h"


CState_Character::CState_Character(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}



HRESULT CState_Character::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);
	m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pCharacter)
		return E_FAIL;

	return S_OK;
}

void CState_Character::Free()
{
	__super::Free();
}

void CState_Character::Neutral_Idle_Input(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
	{
		m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_WALK);
		return;
	}
	if (KEY_TAP(KEY::SPACE))
	{
		m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_JUMP);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_ATTACK_0);
		return;
	}

	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_DASH);
		return;
	}

}

void CState_Character::Battle_Idle_Input(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_WALK);
		return;
	}
	if (KEY_TAP(KEY::SPACE))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_JUMP);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_ATTACK_0);
		return;
	}

	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_DASH);
		return;
	}
}

void CState_Character::Pick_Idle_Input(_float fTimDelta)
{
	if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
	{
		if (CCharacter::STATE::NEUTRAL_PICK_LARGE_IDLE == m_pStateMachineCom->Get_CurrState())
		{
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_WALK);
			return;
		}
		else if (CCharacter::STATE::NEUTRAL_PICK_SMALL_IDLE == m_pStateMachineCom->Get_CurrState())
		{
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_WALK);
			return;
		}
	}
}

void CState_Character::Crouch_Idle_Input(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
	{
		m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_CROUCH_MOVE);
		return;
	}
}

void CState_Character::Neutral_Walk_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
		return;
	}

	_bool bMove = false;
	if (KEY_HOLD(KEY::W))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		bMove = true;
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_JUMP);
	}


	if (KEY_HOLD(KEY::SHIFT))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_RUN);
		return;
	}


	if (KEY_HOLD(KEY::RBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		return;
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
			return;
		}
	}
}

void CState_Character::Battle_Walk_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
		return;
	}

	_bool bMove = false;
	if (KEY_HOLD(KEY::W))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		bMove = true;
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_JUMP);
	}


	if (KEY_HOLD(KEY::SHIFT))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_RUN);
		return;
	}
		

	if (KEY_HOLD(KEY::RBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		return;
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CCharacter::BATTLE_IDLE);
			return;
		}
	}

}

void CState_Character::Pick_Walk_Input(_float fTimeDelta)
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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (KEY_HOLD(KEY::SHIFT))
	{
		if (CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK == m_pStateMachineCom->Get_CurrState())
		{
			m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_RUN);
			return;
		}
			
		else if(CCharacter::STATE::NEUTRAL_PICK_SMALL_WALK == m_pStateMachineCom->Get_CurrState())
		{
			m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_RUN);
			return;
		}
	}



	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			if (CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK == m_pStateMachineCom->Get_CurrState())
			{
				m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE);
				return;
			}

			else if (CCharacter::STATE::NEUTRAL_PICK_SMALL_WALK == m_pStateMachineCom->Get_CurrState())
			{
				m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_IDLE);
				return;
			}
		}
	}
}

void CState_Character::Crouch_Walk_Input(_float fTimeDelta)
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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_WalkSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_CROUCH_MOVE);
			return;
		}
	}

}

void CState_Character::Neutral_Run_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
		return;
	}

	_bool bMove = false;
	if (KEY_HOLD(KEY::W))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		bMove = true;
		m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_JUMP);
	}


	if (KEY_NONE(KEY::SHIFT))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_WALK);
		return;
	}


	if (KEY_HOLD(KEY::RBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		return;
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
			return;
		}
	}
}

void CState_Character::Battle_Run_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
		return;
	}

	_bool bMove = false;
	if (KEY_HOLD(KEY::W))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		bMove = true;
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_JUMP);
	}


	if (KEY_NONE(KEY::SHIFT))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_WALK);
		return;
	}


	if (KEY_HOLD(KEY::RBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		return;
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CCharacter::BATTLE_IDLE);
			return;
		}
	}
}

void CState_Character::Pick_Run_Input(_float fTimeDelta)
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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 4.f * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pCharacter->Get_Default_RunSpeed() * m_pCharacter->Get_Stat().fSpeedWeight, fTimeDelta);

		bMove = true;
	}

	if (KEY_NONE(KEY::SHIFT))
	{
		if (CCharacter::STATE::NEUTRAL_PICK_LARGE_RUN == m_pStateMachineCom->Get_CurrState())
		{
			m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK);
			return;
		}

		else if (CCharacter::STATE::NEUTRAL_PICK_SMALL_RUN == m_pStateMachineCom->Get_CurrState())
		{
			m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_PICK_SMALL_WALK);
			return;
		}
	}



	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			if (CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK == m_pStateMachineCom->Get_CurrState())
			{
				m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE);
				return;
			}

			else if (CCharacter::STATE::NEUTRAL_PICK_SMALL_WALK == m_pStateMachineCom->Get_CurrState())
			{
				m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_IDLE);
				return;
			}
		}
	}
}

void CState_Character::Skill_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::NUM_1))
	{
		CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
		_bool bUseSkill = false;
		switch (eCharacterType)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_PERFECT_BLADE);
			break;
		case CHARACTER_TYPE::ENGINEER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_BURSTCALL);
			break;
		case CHARACTER_TYPE::DESTROYER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_WHEELWIND);
			break;
		}

		if (true == bUseSkill)
			return;
	}

	else if (KEY_TAP(KEY::NUM_2))
	{
		CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
		_bool bUseSkill = false;
		switch (eCharacterType)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_SIPOHONINGLUNGE);
			break;
		case CHARACTER_TYPE::ENGINEER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_ELEMENTAL_BLAST);
			break;
		case CHARACTER_TYPE::DESTROYER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_BRUTALSTRIKE);
			break;
		}

		if (true == bUseSkill)
			return;
	}

	else if (KEY_TAP(KEY::NUM_3))
	{
		CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
		_bool bUseSkill = false;
		switch (eCharacterType)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_SPINNING_ASSAULT);
			break;
		case CHARACTER_TYPE::ENGINEER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_EXPLOSIONSHOT);
			break;
		case CHARACTER_TYPE::DESTROYER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_LEAFSLAM);
			break;
		}

		if (true == bUseSkill)
			return;
	}

	else if (KEY_TAP(KEY::R))
	{
		CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
		_bool bUseSkill = false;
		switch (eCharacterType)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_BURST_MEGA_SLASH);
			break;
		case CHARACTER_TYPE::ENGINEER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINNER_BURST_DESTRUCTION);
			break;
		case CHARACTER_TYPE::DESTROYER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_BURST_HYPERSTRIKE);
			break;
		}

		if (true == bUseSkill)
			return;
	}
}

void CState_Character::Attack_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_DASH);
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		CCharacter::STATE eCurrState = CCharacter::STATE(m_pStateMachineCom->Get_CurrState());
		switch (eCurrState)
		{
		case CCharacter::STATE::BATTLE_ATTACK_0:
			if (m_pModelCom->Get_Progress() >= 0.5f && false == m_pModelCom->Is_Tween())
			{
				m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_1);
				return;
			}
			break;

		case CCharacter::STATE::BATTLE_ATTACK_1:
			if (m_pModelCom->Get_Progress() >= 0.5f && false == m_pModelCom->Is_Tween())
			{
				m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_2);
				return;
			}
			break;

		case CCharacter::STATE::BATTLE_ATTACK_2:
			if (m_pModelCom->Get_Progress() >= 0.5f && false == m_pModelCom->Is_Tween())
			{
				m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_3);
				return;
			}
			break;
		}
	}
}

void CState_Character::Dead_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::R))
	{
		m_pStateMachineCom->Change_State(CCharacter::REVIVE);
	}
}



void CState_Character::Interaction_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::F))
	{
		
	}
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character.h"
#include "UI_Manager.h"


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
		CUI_Manager::GetInstance()->Use_JumpBtn();
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		m_pStateMachineCom->Change_State(CCharacter::BATTLE_ATTACK_0);
		CUI_Manager::GetInstance()->Use_AttackBtn();
		return;
	}

	if (KEY_HOLD(KEY::RBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
		return;
	}

	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_DASH);
		CUI_Manager::GetInstance()->Use_RollBtn();
		return;
	}

	if (KEY_TAP(KEY::F))
	{
		m_pCharacter->PickUp_Target();
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
		CUI_Manager::GetInstance()->Use_JumpBtn();
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		m_pStateMachineCom->Change_State(CCharacter::BATTLE_ATTACK_0);
		CUI_Manager::GetInstance()->Use_AttackBtn();
		return;
	}

	if (KEY_HOLD(KEY::RBTN))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
		return;
	}

	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::BATTLE_DASH);
		CUI_Manager::GetInstance()->Use_RollBtn();
		return;
	}

	if (KEY_TAP(KEY::F))
	{
		m_pCharacter->PickUp_Target();
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

	if (KEY_TAP(KEY::F))
	{
		m_pCharacter->PickDown_Target();
		return;
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
		CUI_Manager::GetInstance()->Use_RollBtn();
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
		CUI_Manager::GetInstance()->Use_JumpBtn();
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
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		CUI_Manager::GetInstance()->Use_AttackBtn();
		return;
	}


	if (KEY_TAP(KEY::F))
	{
		m_pCharacter->PickUp_Target();
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
		CUI_Manager::GetInstance()->Use_RollBtn();
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
		CUI_Manager::GetInstance()->Use_JumpBtn();
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
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		CUI_Manager::GetInstance()->Use_AttackBtn();
		return;
	}


	if (KEY_TAP(KEY::F))
	{
		m_pCharacter->PickUp_Target();
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
		CUI_Manager::GetInstance()->Use_RollBtn();
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
		CUI_Manager::GetInstance()->Use_JumpBtn();
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
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		CUI_Manager::GetInstance()->Use_AttackBtn();
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
		CUI_Manager::GetInstance()->Use_RollBtn();
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
		CUI_Manager::GetInstance()->Use_JumpBtn();
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
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
		CUI_Manager::GetInstance()->Use_AttackBtn();
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
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_ACANE_BARRIER);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(0);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_PERFECT_BLADE);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(0);
			}
			
			break;
		case CHARACTER_TYPE::ENGINEER:
			if (KEY_HOLD(KEY::SHIFT)) 
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_BURSTCALL);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(0);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_FLASH_HEAL);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(0);
			}
			
			break;
		case CHARACTER_TYPE::DESTROYER:
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_FRENGE_CHARGE);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(0);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_WHEELWIND);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(0);
				
			}
			
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
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_FROZEN_STORM);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(1);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_SIPOHONINGLUNGE);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(1);
			}
			break;
		case CHARACTER_TYPE::ENGINEER:
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_TIMELAB);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(1);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_ELEMENTAL_BLAST);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(1);
			}
			break;
		case CHARACTER_TYPE::DESTROYER:
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_BATTLE_CRY);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(1);
				
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_BRUTALSTRIKE);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(1);
			}
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
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_SWORD_TEMPEST);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(2);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_SPINNING_ASSAULT);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(2);
			}
			
			break;
		case CHARACTER_TYPE::ENGINEER:
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_HEALINGTREE);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(2);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINEER_EXPLOSIONSHOT);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(2);
			}
			
			break;
		case CHARACTER_TYPE::DESTROYER:
			if (KEY_HOLD(KEY::SHIFT))
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_IGNORE_PAIN);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ActiveSkillSlot(2);
			}
			else
			{
				bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_LEAFSLAM);
				if (true == bUseSkill)
					CUI_Manager::GetInstance()->Use_ClassSkillSlot(2);
			}
			
			break;
		}

		if (true == bUseSkill)
			return;
	}

	else if (KEY_TAP(KEY::R))
	{
		CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
		_bool bUseSkill = false;

		ELEMENTAL_TYPE eElementalType = m_pCharacter->Get_ElementalType();
		_int iSlotNum = -1;
		if (ELEMENTAL_TYPE::FIRE == eElementalType)
			iSlotNum = 0;
		else if (ELEMENTAL_TYPE::WATER == eElementalType)
			iSlotNum = 1;
		else if (ELEMENTAL_TYPE::WOOD == eElementalType)
			iSlotNum = 2;
		else
			iSlotNum = -1;

		switch (eCharacterType)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::SWORDMAN_BURST_MEGA_SLASH);
			if (true == bUseSkill)
			{
				if (0 > iSlotNum || 2 < iSlotNum)
					return;

				CUI_Manager::GetInstance()->Use_BurstSkillSlot(iSlotNum);
			}
			break;
		case CHARACTER_TYPE::ENGINEER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::ENGINNER_BURST_DESTRUCTION);
			if (true == bUseSkill)
			{
				if (0 > iSlotNum || 2 < iSlotNum)
					return;

				CUI_Manager::GetInstance()->Use_BurstSkillSlot(iSlotNum);
			}
			break;
		case CHARACTER_TYPE::DESTROYER:
			bUseSkill = CSkill_Manager::GetInstance()->Use_Skill(eCharacterType, SKILL_TYPE::DESTROYER_BURST_HYPERSTRIKE);
			if (true == bUseSkill)
			{
				if (0 > iSlotNum || 2 < iSlotNum)
					return;

				CUI_Manager::GetInstance()->Use_BurstSkillSlot(iSlotNum);
			}
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
		CUI_Manager::GetInstance()->Use_RollBtn();
		return;
	}

	if (KEY_TAP(KEY::LBTN))
	{
		//if (CUI_Manager::GetInstance()->Is_UIClicked())
		//	return;

		CCharacter::STATE eCurrState = CCharacter::STATE(m_pStateMachineCom->Get_CurrState());
		CUI_Manager::GetInstance()->Use_AttackBtn();
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
			if (m_pModelCom->Get_Progress() >= 0.3f && false == m_pModelCom->Is_Tween())
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

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Neutral_Walk.h"

CState_Engineer_Neutral_Walk::CState_Engineer_Neutral_Walk(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Neutral_Walk::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Neutral_Walk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Neutral_Walk::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);
}

void CState_Engineer_Neutral_Walk::Exit_State()
{

}

void CState_Engineer_Neutral_Walk::Input(_float fTimeDelta)
{
	if (true == Skill_Input(fTimeDelta))
		return;

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

	if (KEY_TAP(KEY::SPACE))
	{
		bMove = true;
		m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_JUMP);
	}


	if (KEY_HOLD(KEY::SHIFT))
		m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_RUN);

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
			m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
	}
}

CState_Engineer_Neutral_Walk* CState_Engineer_Neutral_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Neutral_Walk* pInstance = new CState_Engineer_Neutral_Walk(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Neutral_Walk");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Neutral_Walk::Free()
{
    __super::Free();
}

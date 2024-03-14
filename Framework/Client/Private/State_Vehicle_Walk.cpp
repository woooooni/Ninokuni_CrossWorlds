#include "stdafx.h"
#include "GameInstance.h"
#include "State_Vehicle_Walk.h"
#include "Vehicle.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Particle_Manager.h"

CState_Vehicle_Walk::CState_Vehicle_Walk(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Walk::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;
    
    return S_OK;
}

void CState_Vehicle_Walk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Vehicle_Walk::Tick_State(_float fTimeDelta)
{
	// 플레이어 움직임이 막혀있다면 return;
	if (false == CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Is_Move_Input())
	{
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;

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

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 10.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

		bMove = true;

	}


	if (KEY_HOLD(KEY::D))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * 10.f * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

		bMove = true;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		bMove = true;
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_JUMP);
		CUI_Manager::GetInstance()->Use_JumpBtn();
	}


	if (KEY_HOLD(KEY::SHIFT))
	{
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
		return;
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
			return;
		}
	}
	else
	{
		GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle_To_Matrix(&m_fEffectAcc, CUtils::Random_Float(0.5f, 1.f), fTimeDelta, 
			TEXT("Particle_Smoke"), m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.2f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
	}
}

void CState_Vehicle_Walk::Exit_State()
{

}

CState_Vehicle_Walk* CState_Vehicle_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Walk* pInstance = new CState_Vehicle_Walk(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Walk");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Walk::Free()
{
    __super::Free();
}

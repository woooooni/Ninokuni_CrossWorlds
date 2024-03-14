#include "stdafx.h"
#include "GameInstance.h"
#include "Animation.h"
#include "State_Vehicle_Jump.h"
#include "Vehicle.h"

CState_Vehicle_Jump::CState_Vehicle_Jump(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Jump::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Vehicle_Jump::Enter_State(void* pArg)
{
    m_pRigidBodyCom->Set_Ground(false);
    m_pRigidBodyCom->Set_Use_Gravity(true);

    Vec4 vPosition = m_pTransformCom->Get_Position();
    vPosition.y += 0.1f;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
    Vec3 vJumpDir = XMVector3Normalize(m_pTransformCom->Get_Look());
    vJumpDir.y = 0.8f;

    m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(vJumpDir), 10.f, true);
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Vehicle_Jump::Tick_State(_float fTimeDelta)
{
    // Idle상태로 돌린다.
    if (true == m_pRigidBodyCom->Is_Ground())
    {
        m_pRigidBodyCom->Set_Velocity(_float3(0.f, 0.f, 0.f));
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
        return;
    }
//    if (m_iCurrAnimIndex == m_AnimIndices[3] &&
//        false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
//    {
//        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
//        return;
//    }

    if (m_iCurrAnimIndex == m_AnimIndices[2] &&
        false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
    {
        m_pRigidBodyCom->Set_Velocity(_float3(0.f, 0.f, 0.f));
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
        return;
    }

    if (0.f > (m_pRigidBodyCom->Get_Velocity()).y && false == m_bJumpDown)
    {
        m_bJumpDown = true;

        m_iCurrAnimIndex = m_AnimIndices[2];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    if (m_iCurrAnimIndex == m_AnimIndices[0] &&
        false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
    {
        m_iCurrAnimIndex = m_AnimIndices[1];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }
 
//    if (m_iCurrAnimIndex == m_AnimIndices[2] &&
//        false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
//    {
//        m_pRigidBodyCom->Set_Velocity(_float3(0.f, 0.f, 0.f));
//        m_iCurrAnimIndex = m_AnimIndices[3];
//        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
//    }
}

void CState_Vehicle_Jump::Exit_State()
{
    m_iCurrAnimIndex = 0;
    m_bJumpDown = false;
}


CState_Vehicle_Jump* CState_Vehicle_Jump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Jump* pInstance = new CState_Vehicle_Jump(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Jump");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Jump::Free()
{
    __super::Free();
}

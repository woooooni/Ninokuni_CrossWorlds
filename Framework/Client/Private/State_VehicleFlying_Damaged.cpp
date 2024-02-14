#include "stdafx.h"
#include "GameInstance.h"

#include "State_VehicleFlying_Damaged.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Camera_Follow.h"

#include "Vehicle.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "Vehicle_Flying_Biplane.h"

CState_VehicleFlying_Damaged::CState_VehicleFlying_Damaged(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Damaged::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pFlying_Vehicle = dynamic_cast<CVehicle_Flying*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pFlying_Vehicle)
        return E_FAIL;

    m_fAccSpeed = 10.f;

    return S_OK;
}

void CState_VehicleFlying_Damaged::Enter_State(void* pArg)
{
    m_fAccSpeed = 30.f;
    m_fAccRadialBlurPower = 0.1f;
    CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Fov(XMConvertToRadians(80.f), 0.5f);

    m_pVehicle->Set_ActiveColliders(CCollider::BODY, false);
    m_pFlying_Vehicle->Set_Infinite(true, 999.f);

    CUIMinigame_Manager::GetInstance()->On_DamagedVignette();

    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    CGameObject* pRider = m_pFlying_Vehicle->Get_Rider();
    if (nullptr != pRider)
        pRider->Get_Component_StateMachine()->Change_State(CCharacter::STATE::FLYING_STAND);
}

void CState_VehicleFlying_Damaged::Tick_State(_float fTimeDelta)
{
    m_fAccRadialBlurPower = max(0.f, m_fAccRadialBlurPower - (0.05f * fTimeDelta));
    m_fAccSpeed -= 10.f * fTimeDelta;

    Vec3 vVelocity = m_pRigidBodyCom->Get_Velocity();
    vVelocity.y = 0.f;

    if (0.3f > vVelocity.Length())
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
        return;
    }

    CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(true, 16.f, m_fAccRadialBlurPower);
    m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f * XMConvertToRadians(180.f) * fTimeDelta);
}

void CState_VehicleFlying_Damaged::Exit_State()
{
    m_fAccSpeed = 10.f;
    m_fAccRadialBlurPower = 0.1f;
    CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Fov(Cam_Fov_Follow_Default, 0.5f);
    CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(false, 16.f);

    m_pRigidBodyCom->Set_FrictionScale(1.f);
    m_pRigidBodyCom->Set_Velocity(Vec3(0.f, 0.f, 0.f));

    m_pFlying_Vehicle->Set_Infinite(true, 3.f);
    m_pVehicle->Set_ActiveColliders(CCollider::BODY, true);
    
}

CState_VehicleFlying_Damaged* CState_VehicleFlying_Damaged::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Damaged* pInstance = new CState_VehicleFlying_Damaged(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Damaged");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Damaged::Free()
{
    __super::Free();
}

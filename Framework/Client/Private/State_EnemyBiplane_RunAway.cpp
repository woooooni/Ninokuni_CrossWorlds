#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_RunAway.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_RunAway::CState_EnemyBiplane_RunAway(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_RunAway::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    return S_OK;
}

void CState_EnemyBiplane_RunAway::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    m_fMovingSpeed = 10.f;
}

void CState_EnemyBiplane_RunAway::Tick_State(_float fTimeDelta)
{
    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom(); //플레이어의 트랜스폼
    if (nullptr == pTargetTransform)
        return;


    m_fMovingSpeed = max(0.f, m_fMovingSpeed - 0.1f * fTimeDelta);

    if (m_fMovingSpeed <= 5.f)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
        return;
    }

    // 일정 체력 이하가 되면, 바로 도망가기.
    Vec3 vDir = XMVector3Normalize(m_pTransformCom->Get_Position() - pTargetTransform->Get_Position());

    Vec3 vLook = Vec3::Lerp(XMVector3Normalize(m_pTransformCom->Get_Look()), vDir, fTimeDelta);

    m_pTransformCom->Rotation_Look(vLook);
    m_pTransformCom->Move(XMVector3Normalize(vDir), m_fMovingSpeed, fTimeDelta);

}

void CState_EnemyBiplane_RunAway::Exit_State()
{
    m_fMovingSpeed = 10.f;
}



CState_EnemyBiplane_RunAway* CState_EnemyBiplane_RunAway::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_RunAway* pInstance = new CState_EnemyBiplane_RunAway(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_RunAway");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_RunAway::Free()
{
    __super::Free();
}

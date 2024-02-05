#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Stand.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Stand::CState_EnemyBiplane_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Stand::Initialize(const list<wstring>& AnimationList)
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

void CState_EnemyBiplane_Stand::Enter_State(void* pArg)
{
    if (true == Check_Use_Skill())
        return;

    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom(); //플레이어의 트랜스폼
    if (nullptr == pTargetTransform)
        return;


    Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();

    _float fDistance = vDir.Length();
    if (fDistance <= m_pEngineerPlane->Get_Attack_StartEnd_Distance().x)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_ATTACK);
        return;
    }

    if (fDistance <= m_pEngineerPlane->Get_Trace_StartEnd_Distance().x)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_TRACE);
        return;
    }

    // 둘다 아니라면, 다시 순회.
    m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_RUN);
    return;
    
    

}

void CState_EnemyBiplane_Stand::Tick_State(_float fTimeDelta)
{
    if (nullptr == m_pTarget)
        return;
    
}

void CState_EnemyBiplane_Stand::Exit_State()
{

}

_bool CState_EnemyBiplane_Stand::Check_Use_Skill()
{
    _float fHpRatio = m_pEngineerPlane->Get_Stat().fCurHP / m_pEngineerPlane->Get_Stat().fMaxHP;
    if (false == m_bLaunch_Pattern1 && fHpRatio <= 0.7f)
    {
        m_bLaunch_Pattern1 = true;
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_0);
        return true;
    }

    if (false == m_bLaunch_Pattern2 && fHpRatio <= 0.5f)
    {
        m_bLaunch_Pattern2 = true;
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_1);
        return true;
    }

    if (false == m_bLaunch_Pattern3 && fHpRatio <= 0.3f)
    {
        m_bLaunch_Pattern3 = true;
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_2);
        return true;
    }

    return false;
}


CState_EnemyBiplane_Stand* CState_EnemyBiplane_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Stand* pInstance = new CState_EnemyBiplane_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Stand::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Stand.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Riding_Manager.h"
#include "Vehicle_Flying_EnemyBiplane.h"
#include "Vehicle_Flying_Biplane.h"

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
    
    
    // TODO : 플레이어 포지션 변경.
    // CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform()->Set_Position()
    // CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component<CPhysX_Controller>(L"Com_Controller")->Set_EnterLevel_Position()

    // TODO : 내 포지션 변경.
    m_pTransformCom->Set_Position(Vec4(-0.65f, 111.44f, 288.f, 1.f));

    // TODO : camera target


    // TODO : 페이드.

    return S_OK;
}

void CState_EnemyBiplane_Stand::Enter_State(void* pArg)
{
    m_pEngineerPlane->Set_Infinite(false, 0.f);
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

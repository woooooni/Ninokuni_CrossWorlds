#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Dead.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Dead::CState_EnemyBiplane_Dead(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Dead::Initialize(const list<wstring>& AnimationList)
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

void CState_EnemyBiplane_Dead::Enter_State(void* pArg)
{
    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom(); //플레이어의 트랜스폼
    if (nullptr == pTargetTransform)
        return;


    Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();

    _float fDistance = vDir.Length();
    if (fDistance <= m_pEngineerPlane->Get_Attack_StartEnd_Distance().x)
    {
        // Attack 스테이트로 변경.
        return;
    }

    if (fDistance <= m_pEngineerPlane->Get_Trace_StartEnd_Distance().x)
    {
        // Trace 스테이트로 변경.
        return;
    }

}

void CState_EnemyBiplane_Dead::Tick_State(_float fTimeDelta)
{
    if (nullptr == m_pTarget)
        return;
    
}

void CState_EnemyBiplane_Dead::Exit_State()
{

}


CState_EnemyBiplane_Dead* CState_EnemyBiplane_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Dead* pInstance = new CState_EnemyBiplane_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Dead::Free()
{
    __super::Free();
}

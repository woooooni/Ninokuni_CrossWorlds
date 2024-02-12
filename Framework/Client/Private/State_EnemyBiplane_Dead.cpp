#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Dead.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Grandprix_Manager.h"

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
    m_pEngineerPlane->Reserve_Dead(true);
    m_pEngineerPlane->Get_Rider()->Reserve_Dead(true);
    CGrandprix_Manager::GetInstance()->Show_GoalObject();

}

void CState_EnemyBiplane_Dead::Tick_State(_float fTimeDelta)
{
       
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

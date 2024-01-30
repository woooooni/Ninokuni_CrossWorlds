#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy_VehicleFlying_Attack.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"

CState_Enemy_VehicleFlying_Attack::CState_Enemy_VehicleFlying_Attack(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_Enemy_VehicleFlying_Attack::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Enemy_VehicleFlying_Attack::Tick_State(_float fTimeDelta)
{

}

void CState_Enemy_VehicleFlying_Attack::Exit_State()
{

}

CState_Enemy_VehicleFlying_Attack* CState_Enemy_VehicleFlying_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Attack* pInstance = new CState_Enemy_VehicleFlying_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Attack::Free()
{
    __super::Free();
}

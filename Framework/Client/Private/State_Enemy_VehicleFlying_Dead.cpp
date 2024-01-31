#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy_VehicleFlying_Dead.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"

CState_Enemy_VehicleFlying_Dead::CState_Enemy_VehicleFlying_Dead(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Dead::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_Enemy_VehicleFlying_Dead::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Enemy_VehicleFlying_Dead::Tick_State(_float fTimeDelta)
{

}

void CState_Enemy_VehicleFlying_Dead::Exit_State()
{

}

CState_Enemy_VehicleFlying_Dead* CState_Enemy_VehicleFlying_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Dead* pInstance = new CState_Enemy_VehicleFlying_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Dead::Free()
{
    __super::Free();
}

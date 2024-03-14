#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_Enemy_VehicleFlying_Stand.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"

#include "UIMinigame_Manager.h"

CState_Enemy_VehicleFlying_Stand::CState_Enemy_VehicleFlying_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;
    
    return S_OK;
}

void CState_Enemy_VehicleFlying_Stand::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Enemy_VehicleFlying_Stand::Tick_State(_float fTimeDelta)
{

}

void CState_Enemy_VehicleFlying_Stand::Exit_State()
{

}



CState_Enemy_VehicleFlying_Stand* CState_Enemy_VehicleFlying_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Stand* pInstance = new CState_Enemy_VehicleFlying_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Stand::Free()
{
    __super::Free();
}

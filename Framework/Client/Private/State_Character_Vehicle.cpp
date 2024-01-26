#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Vehicle.h"
#include "Riding_Manager.h"

CState_Character_Vehicle::CState_Character_Vehicle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Vehicle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Vehicle::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_pRigidBodyCom->Set_Use_Gravity(false);
    m_pRigidBodyCom->Set_Ground(true);
}

void CState_Character_Vehicle::Tick_State(_float fTimeDelta)
{
    if (KEY_TAP(KEY::V))
    {
        CRiding_Manager::GetInstance()->Ride(CRiding_Manager::UDADAK, false);
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
        return;
    }

    __super::OnBoard_Input(fTimeDelta);
}

void CState_Character_Vehicle::Exit_State()
{
    //m_pCharacter->Increase_HP(1000000);
    m_pRigidBodyCom->Set_Use_Gravity(true);
    m_pRigidBodyCom->Set_Ground(false);
}



CState_Character_Vehicle* CState_Character_Vehicle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Vehicle* pInstance = new CState_Character_Vehicle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Vehicle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Vehicle::Free()
{
    __super::Free();
}

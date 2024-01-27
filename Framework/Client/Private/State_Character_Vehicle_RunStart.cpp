#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Vehicle_RunStart.h"
#include "Riding_Manager.h"

CState_Character_Vehicle_RunStart::CState_Character_Vehicle_RunStart(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Vehicle_RunStart::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Vehicle_RunStart::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_pRigidBodyCom->Set_Use_Gravity(false);
    m_pRigidBodyCom->Set_Ground(true);
}

void CState_Character_Vehicle_RunStart::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::VEHICLE_STAND);

    __super::OnBoard_Input(fTimeDelta);
}

void CState_Character_Vehicle_RunStart::Exit_State()
{
//   m_pRigidBodyCom->Set_Use_Gravity(true);
//   m_pRigidBodyCom->Set_Ground(false);
}



CState_Character_Vehicle_RunStart* CState_Character_Vehicle_RunStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Vehicle_RunStart* pInstance = new CState_Character_Vehicle_RunStart(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Vehicle_RunStart");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Vehicle_RunStart::Free()
{
    __super::Free();
}

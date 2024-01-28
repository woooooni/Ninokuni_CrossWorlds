#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_DoorEnter.h"
#include "Riding_Manager.h"

CState_Character_DoorEnter::CState_Character_DoorEnter(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_DoorEnter::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_DoorEnter::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pModelCom->Set_KeyFrame_By_Progress(0.7f);

    // 탈 것으로 인한 코드 추가
    CRiding_Manager::GetInstance()->Ride_ForCharacter(CRiding_Manager::VEHICLE_TYPE::UDADAK, false);
    m_pRigidBodyCom->Set_Use_Gravity(true);
    m_pRigidBodyCom->Set_Ground(false);
}

void CState_Character_DoorEnter::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
}

void CState_Character_DoorEnter::Exit_State()
{
}

CState_Character_DoorEnter* CState_Character_DoorEnter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_DoorEnter* pInstance = new CState_Character_DoorEnter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_DoorEnter::Free()
{
    __super::Free();
}

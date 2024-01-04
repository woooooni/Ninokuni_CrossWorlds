#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_DoorEnter.h"

CState_SwordMan_DoorEnter::CState_SwordMan_DoorEnter(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_DoorEnter::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_DoorEnter::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_DoorEnter::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
}

void CState_SwordMan_DoorEnter::Exit_State()
{
    
}

CState_SwordMan_DoorEnter* CState_SwordMan_DoorEnter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_DoorEnter* pInstance = new CState_SwordMan_DoorEnter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_DoorEnter::Free()
{
    __super::Free();
}

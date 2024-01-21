#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_BurstSkill_Destruction.h"

CState_Engineer_BurstSkill_Destruction::CState_Engineer_BurstSkill_Destruction(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_BurstSkill_Destruction::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_BurstSkill_Destruction::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();
    m_pCharacter->Appear_Weapon();
}

void CState_Engineer_BurstSkill_Destruction::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_BurstSkill_Destruction::Exit_State()
{
    
}
CState_Engineer_BurstSkill_Destruction* CState_Engineer_BurstSkill_Destruction::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_BurstSkill_Destruction* pInstance = new CState_Engineer_BurstSkill_Destruction(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_BurstSkill_Destruction");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_BurstSkill_Destruction::Free()
{
    __super::Free();
}

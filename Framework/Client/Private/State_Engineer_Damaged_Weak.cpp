#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Damaged_Weak.h"

CState_Engineer_Damaged_Weak::CState_Engineer_Damaged_Weak(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Damaged_Weak::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Damaged_Weak::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Damaged_Weak::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_Damaged_Weak::Exit_State()
{
    
}


CState_Engineer_Damaged_Weak* CState_Engineer_Damaged_Weak::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Damaged_Weak* pInstance = new CState_Engineer_Damaged_Weak(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Damaged_Weak::Free()
{
    __super::Free();
}

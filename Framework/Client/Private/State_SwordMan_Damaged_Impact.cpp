#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Damaged_Impact.h"

CState_SwordMan_Damaged_Impact::CState_SwordMan_Damaged_Impact(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Damaged_Impact::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Damaged_Impact::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_fAccRecovery = 0.f;
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_SwordMan_Damaged_Impact::Tick_State(_float fTimeDelta)
{
    if (m_iCurrAnimIndex == m_AnimIndices[0])
    {
        if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        {
            m_iCurrAnimIndex = m_AnimIndices[1];
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
            return;
        }
    }
    else if (m_iCurrAnimIndex == m_AnimIndices[1])
    {
        m_fAccRecovery += fTimeDelta;
        if (m_fAccRecovery >= m_fRecoveryTime)
        {
            m_fAccRecovery = 0.f;
            m_iCurrAnimIndex = m_AnimIndices[2];
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
            return;
        }
    }
    else if(m_iCurrAnimIndex == m_AnimIndices[2])
    {
        if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
            return;
        }
            
    }
    
}

void CState_SwordMan_Damaged_Impact::Exit_State()
{
    m_iCurrAnimIndex = 0;
    m_fAccRecovery = 0.f;

}

CState_SwordMan_Damaged_Impact* CState_SwordMan_Damaged_Impact::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Damaged_Impact* pInstance = new CState_SwordMan_Damaged_Impact(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Damaged_Impact::Free()
{
    __super::Free();
}
#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Battle_Idle.h"

CState_Character_Battle_Idle::CState_Character_Battle_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Battle_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Battle_Idle::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_fAccReturnNuetral = 0.f;
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Character_Battle_Idle::Tick_State(_float fTimeDelta)
{
    m_fAccReturnNuetral += fTimeDelta;
    if (m_fAccReturnNuetral >= m_fReturnNuetralTime)
    {
        m_fAccReturnNuetral = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[1];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    if (m_iCurrAnimIndex == m_AnimIndices[1])
    {
        if (m_pModelCom->Get_CurrAnimationFrame() >= 18.f)
        {
            m_pCharacter->Disappear_Weapon();
        }

        if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
        }
    } 
}

void CState_Character_Battle_Idle::Exit_State()
{
    m_fAccReturnNuetral = 0.f;
}


CState_Character_Battle_Idle* CState_Character_Battle_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Battle_Idle* pInstance = new CState_Character_Battle_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Battle_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Battle_Idle::Free()
{
    __super::Free();
}

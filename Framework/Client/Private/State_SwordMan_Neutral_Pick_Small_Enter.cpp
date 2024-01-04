#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Small_Enter.h"

CState_SwordMan_Neutral_Pick_Small_Enter::CState_SwordMan_Neutral_Pick_Small_Enter(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Small_Enter::Initialize(const list<wstring>& AnimationList)
{
    
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Small_Enter::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Small_Enter::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_IDLE);
}

void CState_SwordMan_Neutral_Pick_Small_Enter::Exit_State()
{

}


CState_SwordMan_Neutral_Pick_Small_Enter* CState_SwordMan_Neutral_Pick_Small_Enter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Small_Enter* pInstance = new CState_SwordMan_Neutral_Pick_Small_Enter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Small_Enter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Small_Enter::Free()
{
    __super::Free();
}

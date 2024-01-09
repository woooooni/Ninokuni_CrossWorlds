#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Pick_Small_Throw.h"

CState_Character_Neutral_Pick_Small_Throw::CState_Character_Neutral_Pick_Small_Throw(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Pick_Small_Throw::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    return S_OK;
}

void CState_Character_Neutral_Pick_Small_Throw::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Neutral_Pick_Small_Throw::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
}

void CState_Character_Neutral_Pick_Small_Throw::Exit_State()
{

}


CState_Character_Neutral_Pick_Small_Throw* CState_Character_Neutral_Pick_Small_Throw::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Pick_Small_Throw* pInstance = new CState_Character_Neutral_Pick_Small_Throw(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Pick_Small_Throw");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Pick_Small_Throw::Free()
{
    __super::Free();
}

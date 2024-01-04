#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Neutral_Pick_Large_Enter.h"

CState_Engineer_Neutral_Pick_Large_Enter::CState_Engineer_Neutral_Pick_Large_Enter(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Neutral_Pick_Large_Enter::Initialize(const list<wstring>& AnimationList)
{
    
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_Engineer_Neutral_Pick_Large_Enter::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Neutral_Pick_Large_Enter::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE);
}

void CState_Engineer_Neutral_Pick_Large_Enter::Exit_State()
{

}


CState_Engineer_Neutral_Pick_Large_Enter* CState_Engineer_Neutral_Pick_Large_Enter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Neutral_Pick_Large_Enter* pInstance = new CState_Engineer_Neutral_Pick_Large_Enter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Neutral_Pick_Large_Enter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Neutral_Pick_Large_Enter::Free()
{
    __super::Free();
}

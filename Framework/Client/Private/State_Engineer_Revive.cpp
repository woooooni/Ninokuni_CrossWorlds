#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Revive.h"

CState_Engineer_Revive::CState_Engineer_Revive(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Revive::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Revive::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Revive::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_Revive::Exit_State()
{

}



CState_Engineer_Revive* CState_Engineer_Revive::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Revive* pInstance = new CState_Engineer_Revive(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Revive");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Revive::Free()
{
    __super::Free();
}

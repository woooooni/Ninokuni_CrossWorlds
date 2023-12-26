#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Dead.h"

CState_Engineer_Dead::CState_Engineer_Dead(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Dead::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Dead::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Dead::Tick_State(_float fTimeDelta)
{
    
}

void CState_Engineer_Dead::Exit_State()
{

}

void CState_Engineer_Dead::Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::R))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::REVIVE);
    }
}

CState_Engineer_Dead* CState_Engineer_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Dead* pInstance = new CState_Engineer_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Dead::Free()
{
    __super::Free();
}

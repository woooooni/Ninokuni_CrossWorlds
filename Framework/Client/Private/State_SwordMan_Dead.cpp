#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Dead.h"

CState_SwordMan_Dead::CState_SwordMan_Dead(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Dead::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Dead::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Dead::Tick_State(_float fTimeDelta)
{
    
}

void CState_SwordMan_Dead::Exit_State()
{

}

void CState_SwordMan_Dead::Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::R))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::REVIVE);
    }
}

CState_SwordMan_Dead* CState_SwordMan_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Dead* pInstance = new CState_SwordMan_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Dead::Free()
{
    __super::Free();
}

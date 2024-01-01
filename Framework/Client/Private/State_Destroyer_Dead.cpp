#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Dead.h"

CState_Destroyer_Dead::CState_Destroyer_Dead(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Dead::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Dead::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_Dead::Tick_State(_float fTimeDelta)
{
    
}

void CState_Destroyer_Dead::Exit_State()
{

}

void CState_Destroyer_Dead::Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::R))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::REVIVE);
    }
}

CState_Destroyer_Dead* CState_Destroyer_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Dead* pInstance = new CState_Destroyer_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Dead::Free()
{
    __super::Free();
}

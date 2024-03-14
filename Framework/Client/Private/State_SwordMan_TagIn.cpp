#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_TagIn.h"

CState_SwordMan_TagIn::CState_SwordMan_TagIn(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_TagIn::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_TagIn::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_TagIn::Tick_State(_float fTimeDelta)
{
    m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
}

void CState_SwordMan_TagIn::Exit_State()
{

}

CState_SwordMan_TagIn* CState_SwordMan_TagIn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_TagIn* pInstance = new CState_SwordMan_TagIn(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_TagIn");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_TagIn::Free()
{
    __super::Free();
}

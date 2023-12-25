#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_AbNormality_Stun.h"

CState_SwordMan_AbNormality_Stun::CState_SwordMan_AbNormality_Stun(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_AbNormality_Stun::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_SwordMan_AbNormality_Stun::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_AbNormality_Stun::Tick_State(_float fTimeDelta)
{
    
}

void CState_SwordMan_AbNormality_Stun::Exit_State()
{
    
}


CState_SwordMan_AbNormality_Stun* CState_SwordMan_AbNormality_Stun::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_AbNormality_Stun* pInstance = new CState_SwordMan_AbNormality_Stun(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_AbNormality_Stun");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_AbNormality_Stun::Free()
{
    __super::Free();
}

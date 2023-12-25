#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Small_Idle.h"

CState_SwordMan_Neutral_Pick_Small_Idle::CState_SwordMan_Neutral_Pick_Small_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Small_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Exit_State()
{

}

void CState_SwordMan_Neutral_Pick_Small_Idle::Input(_float fTimeDelta)
{

}


CState_SwordMan_Neutral_Pick_Small_Idle* CState_SwordMan_Neutral_Pick_Small_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Small_Idle* pInstance = new CState_SwordMan_Neutral_Pick_Small_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Small_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Free()
{
    __super::Free();
}

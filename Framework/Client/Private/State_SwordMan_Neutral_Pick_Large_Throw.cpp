#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Large_Throw.h"

CState_SwordMan_Neutral_Pick_Large_Throw::CState_SwordMan_Neutral_Pick_Large_Throw(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Large_Throw::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Large_Throw::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Large_Throw::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
}

void CState_SwordMan_Neutral_Pick_Large_Throw::Exit_State()
{

}


CState_SwordMan_Neutral_Pick_Large_Throw* CState_SwordMan_Neutral_Pick_Large_Throw::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Large_Throw* pInstance = new CState_SwordMan_Neutral_Pick_Large_Throw(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Large_Throw");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Large_Throw::Free()
{
    __super::Free();
}

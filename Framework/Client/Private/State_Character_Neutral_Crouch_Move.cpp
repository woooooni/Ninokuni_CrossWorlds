#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Crouch_Move.h"

CState_Character_Neutral_Crouch_Move::CState_Character_Neutral_Crouch_Move(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Crouch_Move::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Crouch_Move::Enter_State(void* pArg)
{
	m_pCharacter->Disappear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Neutral_Crouch_Move::Tick_State(_float fTimeDelta)
{
	__super::Crouch_Walk_Input(fTimeDelta);
}

void CState_Character_Neutral_Crouch_Move::Exit_State()
{

}



CState_Character_Neutral_Crouch_Move* CState_Character_Neutral_Crouch_Move::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Crouch_Move* pInstance = new CState_Character_Neutral_Crouch_Move(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Crouch_Move");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Crouch_Move::Free()
{
    __super::Free();
}

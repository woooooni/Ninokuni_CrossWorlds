#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Battle_Walk.h"

CState_Character_Battle_Walk::CState_Character_Battle_Walk(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Battle_Walk::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Battle_Walk::Enter_State(void* pArg)
{
	m_pCharacter->Appear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Battle_Walk::Tick_State(_float fTimeDelta)
{

}

void CState_Character_Battle_Walk::Exit_State()
{

}

CState_Character_Battle_Walk* CState_Character_Battle_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Battle_Walk* pInstance = new CState_Character_Battle_Walk(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Battle_Walk");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Battle_Walk::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Battle_Run.h"

CState_Character_Battle_Run::CState_Character_Battle_Run(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Battle_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Battle_Run::Enter_State(void* pArg)
{
	m_pCharacter->Appear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Battle_Run::Tick_State(_float fTimeDelta)
{
	
}

void CState_Character_Battle_Run::Exit_State()
{

}


CState_Character_Battle_Run* CState_Character_Battle_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Battle_Run* pInstance = new CState_Character_Battle_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Battle_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Battle_Run::Free()
{
    __super::Free();
}

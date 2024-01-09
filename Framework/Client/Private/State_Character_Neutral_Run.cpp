#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Run.h"

CState_Character_Neutral_Run::CState_Character_Neutral_Run(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Run::Enter_State(void* pArg)
{
	m_pCharacter->Disappear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Neutral_Run::Tick_State(_float fTimeDelta)
{
    __super::Neutral_Run_Input(fTimeDelta);
    __super::Skill_Input(fTimeDelta);
	
}

void CState_Character_Neutral_Run::Exit_State()
{

}

CState_Character_Neutral_Run* CState_Character_Neutral_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Run* pInstance = new CState_Character_Neutral_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Run::Free()
{
    __super::Free();
}

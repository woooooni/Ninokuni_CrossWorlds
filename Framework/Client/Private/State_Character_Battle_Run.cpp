#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Battle_Run.h"
#include "Particle_Manager.h"
#include "Utils.h"

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
    GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fEffectAcc, CUtils::Random_Float(0.2f, 0.4f), fTimeDelta, TEXT("Particle_Smoke"), m_pCharacter, _float3(CUtils::Random_Float(-0.1f, 0.1f), CUtils::Random_Float(0.2f, 0.4f), CUtils::Random_Float(-0.1f, 0.1f)));

    __super::Battle_Run_Input(fTimeDelta);
    __super::Skill_Input(fTimeDelta);
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

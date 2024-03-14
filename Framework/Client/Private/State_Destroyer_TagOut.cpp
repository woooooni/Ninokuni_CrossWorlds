#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_TagOut.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Particle_Manager.h"

CState_Destroyer_TagOut::CState_Destroyer_TagOut(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_TagOut::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_TagOut::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Change_Player"), 
        m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 1.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
}

void CState_Destroyer_TagOut::Tick_State(_float fTimeDelta)
{
    // TODO :: 조건 걸어서..
    CGame_Manager::GetInstance()->Get_Player()->Switch_Tag_Character();
}

void CState_Destroyer_TagOut::Exit_State()
{

}

CState_Destroyer_TagOut* CState_Destroyer_TagOut::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_TagOut* pInstance = new CState_Destroyer_TagOut(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_TagOut");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_TagOut::Free()
{
    __super::Free();
}

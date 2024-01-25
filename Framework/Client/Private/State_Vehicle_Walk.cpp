#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Vehicle_Walk.h"
#include "Particle_Manager.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_Vehicle_Walk::CState_Vehicle_Walk(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Walk::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Vehicle_Walk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

    if (CGame_Manager::GetInstance()->Get_Kuu() != nullptr)
        CGame_Manager::GetInstance()->Get_Kuu()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CKuu::NPC_UNIQUENPC_WALK);
}

void CState_Vehicle_Walk::Tick_State(_float fTimeDelta)
{
    
}

void CState_Vehicle_Walk::Exit_State()
{

}

CState_Vehicle_Walk* CState_Vehicle_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Walk* pInstance = new CState_Vehicle_Walk(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Walk");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Walk::Free()
{
    __super::Free();
}

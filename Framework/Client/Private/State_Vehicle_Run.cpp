#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Vehicle_Run.h"
#include "Particle_Manager.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_Vehicle_Run::CState_Vehicle_Run(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Vehicle_Run::Enter_State(void* pArg)
{
    if (CGame_Manager::GetInstance()->Get_Kuu() != nullptr)
        CGame_Manager::GetInstance()->Get_Kuu()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CKuu::NPC_UNIQUENPC_RUN);
}

void CState_Vehicle_Run::Tick_State(_float fTimeDelta)
{
	
}

void CState_Vehicle_Run::Exit_State()
{

}

CState_Vehicle_Run* CState_Vehicle_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Run* pInstance = new CState_Vehicle_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Run::Free()
{
    __super::Free();
}

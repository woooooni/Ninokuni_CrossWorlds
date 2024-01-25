#include "stdafx.h"
#include "GameInstance.h"
#include "State_Vehicle_Sprint.h"


CState_Vehicle_Sprint::CState_Vehicle_Sprint(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Sprint::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Vehicle_Sprint::Enter_State(void* pArg)
{

}

void CState_Vehicle_Sprint::Tick_State(_float fTimeDelta)
{
	
}

void CState_Vehicle_Sprint::Exit_State()
{

}

CState_Vehicle_Sprint* CState_Vehicle_Sprint::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Sprint* pInstance = new CState_Vehicle_Sprint(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Sprint");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Sprint::Free()
{
    __super::Free();
}

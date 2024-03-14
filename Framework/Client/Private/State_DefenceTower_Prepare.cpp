#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_DefenceTower_Prepare.h"

CState_DefenceTower_Prepare::CState_DefenceTower_Prepare(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_DefenceTower_Prepare::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_DefenceTower_Prepare::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_DefenceTower_Prepare::Tick_State(_float fTimeDelta)
{
    
}

void CState_DefenceTower_Prepare::Exit_State()
{

}

CState_DefenceTower_Prepare* CState_DefenceTower_Prepare::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_DefenceTower_Prepare* pInstance = new CState_DefenceTower_Prepare(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_DefenceTower_Prepare");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_DefenceTower_Prepare::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_DefenceTower_Dead.h"

CState_DefenceTower_Dead::CState_DefenceTower_Dead(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_DefenceTower_Dead::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_DefenceTower_Dead::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_DefenceTower_Dead::Tick_State(_float fTimeDelta)
{
    
}

void CState_DefenceTower_Dead::Exit_State()
{

}

CState_DefenceTower_Dead* CState_DefenceTower_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_DefenceTower_Dead* pInstance = new CState_DefenceTower_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_DefenceTower_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_DefenceTower_Dead::Free()
{
    __super::Free();
}

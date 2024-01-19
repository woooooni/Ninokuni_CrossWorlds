#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "State_DefenceTower_Idle.h"
#include "UI_Manager.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_DefenceTower_Idle::CState_DefenceTower_Idle(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_DefenceTower_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_DefenceTower_Idle::Enter_State(void* pArg)
{
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_DefenceTower_Idle::Tick_State(_float fTimeDelta)
{
    
}

void CState_DefenceTower_Idle::Exit_State()
{

}



CState_DefenceTower_Idle* CState_DefenceTower_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_DefenceTower_Idle* pInstance = new CState_DefenceTower_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_DefenceTower_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_DefenceTower_Idle::Free()
{
    __super::Free();
}

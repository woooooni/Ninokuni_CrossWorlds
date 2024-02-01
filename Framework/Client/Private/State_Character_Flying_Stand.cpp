#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Flying_Stand.h"
#include "Riding_Manager.h"

CState_Character_Flying_Stand::CState_Character_Flying_Stand(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Flying_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Flying_Stand::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_pRigidBodyCom->Set_Use_Gravity(false);
    m_pRigidBodyCom->Set_Ground(true);
}

void CState_Character_Flying_Stand::Tick_State(_float fTimeDelta)
{
    __super::OnBoard_Input(fTimeDelta);
}

void CState_Character_Flying_Stand::Exit_State()
{
    m_pRigidBodyCom->Set_Use_Gravity(true);
    m_pRigidBodyCom->Set_Ground(false);
}



CState_Character_Flying_Stand* CState_Character_Flying_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Flying_Stand* pInstance = new CState_Character_Flying_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Flying_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Flying_Stand::Free()
{
    __super::Free();
}

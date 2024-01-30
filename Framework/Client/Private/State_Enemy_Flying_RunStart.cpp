#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Enemy_Flying_RunStart.h"
#include "Riding_Manager.h"

CState_Enemy_Flying_RunStart::CState_Enemy_Flying_RunStart(CStateMachine* pMachine)
    : CState_Enemy(pMachine)
{
}

HRESULT CState_Enemy_Flying_RunStart::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Enemy_Flying_RunStart::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_pRigidBodyCom->Set_Use_Gravity(false);
    m_pRigidBodyCom->Set_Ground(true);
}

void CState_Enemy_Flying_RunStart::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::FLYING_STAND);

}

void CState_Enemy_Flying_RunStart::Exit_State()
{
//   m_pRigidBodyCom->Set_Use_Gravity(true);
//   m_pRigidBodyCom->Set_Ground(false);
}



CState_Enemy_Flying_RunStart* CState_Enemy_Flying_RunStart::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_Flying_RunStart* pInstance = new CState_Enemy_Flying_RunStart(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_Flying_RunStart");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_Flying_RunStart::Free()
{
    __super::Free();
}

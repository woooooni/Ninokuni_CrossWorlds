#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Defence_Tower.h"
#include "State_ShadowTower_Attack.h"

CState_ShadowTower_Attack::CState_ShadowTower_Attack(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_ShadowTower_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_ShadowTower_Attack::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_ShadowTower_Attack::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() == 5)
        Fire();

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CDefence_Tower::DEFENCE_TOWER_STATE::TOWER_STATE_IDLE);
        

}

void CState_ShadowTower_Attack::Exit_State()
{

}

void CState_ShadowTower_Attack::Fire()
{

}

CState_ShadowTower_Attack* CState_ShadowTower_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_ShadowTower_Attack* pInstance = new CState_ShadowTower_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_ShadowTower_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_ShadowTower_Attack::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Skill_2.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Skill_2::CState_EnemyBiplane_Skill_2(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Skill_2::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    return S_OK;
}

void CState_EnemyBiplane_Skill_2::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_EnemyBiplane_Skill_2::Tick_State(_float fTimeDelta)
{
    
}

void CState_EnemyBiplane_Skill_2::Exit_State()
{
    
}


CState_EnemyBiplane_Skill_2* CState_EnemyBiplane_Skill_2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Skill_2* pInstance = new CState_EnemyBiplane_Skill_2(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Skill_2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Skill_2::Free()
{
    __super::Free();
}

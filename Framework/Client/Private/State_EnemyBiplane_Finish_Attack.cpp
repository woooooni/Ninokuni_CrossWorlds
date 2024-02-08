#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Finish_Attack.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Finish_Attack::CState_EnemyBiplane_Finish_Attack(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Finish_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    /*m_pComputeShader = CShader::Create(GI->Get_Device(), GI->Get_Context(), L"../Bin/ShaderFiles/")
    
    GI->Get_Device()->create*/

    

    return S_OK;
}

void CState_EnemyBiplane_Finish_Attack::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    m_bUpdate = false;
}

void CState_EnemyBiplane_Finish_Attack::Tick_State(_float fTimeDelta)
{
    m_fAccGenMissile += fTimeDelta;
    if (m_fAccGenMissile >= m_fGenMissileTime)
    {
        m_fAccGenMissile = 0.f;
        // 미사일을 만들고, 벡터에 푸시백.
    }

    if (m_Missiles.size() >= 100)
    {
        // 일정 미사일이상 채워지면 한번에 Launch.
    }
}

void CState_EnemyBiplane_Finish_Attack::Exit_State()
{

}


CState_EnemyBiplane_Finish_Attack* CState_EnemyBiplane_Finish_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Finish_Attack* pInstance = new CState_EnemyBiplane_Finish_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Finish_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Finish_Attack::Free()
{
    __super::Free();
}

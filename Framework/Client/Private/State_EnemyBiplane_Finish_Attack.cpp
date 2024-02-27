#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Finish_Attack.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_Biplane.h"
#include "Vehicle_Flying_EnemyBiplane.h"
#include "Grandprix_Manager.h"

#include "Riding_Manager.h"

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

    

//    ID3DBlob* pComputeShaderBlob = nullptr;
//    ID3DBlob* pErrBlob = nullptr;
//    _int iFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef NDEBUG
//    iFlags |= D3DCOMPILE_DEBUG;
//#endif // _DEBUG
//
//    if (FAILED(D3DCompileFromFile(L"../Bin/ShaderFiles/Shader_Compute.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS_MAIN_COMPUTE_ENGINEER_FINISH", "cs_5_0", iFlags, 0, &pComputeShaderBlob, &pErrBlob)))
//    {
//        MSG_BOX("Compile ComputeShader Failed. : CState_EnemyBiplane_Finish_Attack::Initialize.");
//        return E_FAIL;
//    }
//    
//    if (FAILED(GI->Get_Device()->CreateComputeShader(pComputeShaderBlob->GetBufferPointer(), pComputeShaderBlob->GetBufferSize(), nullptr, &m_pComputeShader)))
//    {
//        MSG_BOX("Create ComputeShader Failed. : CState_EnemyBiplane_Finish_Attack::Initialize.");
//        return E_FAIL;
//    }
//
//    GI->Get_Context()->CSSetShader(m_pComputeShader, nullptr, 0);
//    GI->Get_Context()->Dispatch(1, 64, 1);



    return S_OK;
}

void CState_EnemyBiplane_Finish_Attack::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    m_bUpdate = false;

    if (nullptr == m_pTarget)
    {
        m_pTarget = CRiding_Manager::GetInstance()->Get_Character_Biplane();
    }
}

void CState_EnemyBiplane_Finish_Attack::Tick_State(_float fTimeDelta)
{
    m_fAccGenMissile += fTimeDelta;
    if (m_fAccGenMissile >= m_fGenMissileTime)
    {
        m_fAccGenMissile = 0.f;
        // �̻����� �����, ���Ϳ� Ǫ�ù�.
    }

    if (m_Missiles.size() >= 100)
    {
        // ���� �̻����̻� ä������ �ѹ��� Launch.
    }
}

void CState_EnemyBiplane_Finish_Attack::Exit_State()
{
    CGrandprix_Manager::GetInstance()->Show_GoalObject();
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
    Safe_Release(m_pComputeShader);
}

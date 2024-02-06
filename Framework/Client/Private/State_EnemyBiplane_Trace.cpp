#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Trace.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Trace::CState_EnemyBiplane_Trace(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Trace::Initialize(const list<wstring>& AnimationList)
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

void CState_EnemyBiplane_Trace::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_EnemyBiplane_Trace::Tick_State(_float fTimeDelta)
{
    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom(); //플레이어의 트랜스폼
    if (nullptr == pTargetTransform)
        return;


    Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();

    _float fDistance = vDir.Length();
    if (fDistance <= m_pEngineerPlane->Get_Attack_StartEnd_Distance().x)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_ATTACK);
        return;
    }

    if (fDistance > m_pEngineerPlane->Get_Trace_StartEnd_Distance().y)
    {
        // Stand, 혹은 Run 스테이트로 변경.
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
        return;
    }

    // 둘다 아니라면 추적 상태를 유지하며, Look을 서서히 바꿔준다.
    Vec3 vLook = Vec3::Lerp(XMVector3Normalize(m_pTransformCom->Get_Look()), vDir, fTimeDelta);

    m_pTransformCom->Rotation_Look(vLook);
    m_pTransformCom->Move(XMVector3Normalize(vDir), m_fMovingSpeed, fTimeDelta);
}

void CState_EnemyBiplane_Trace::Exit_State()
{
    
}


CState_EnemyBiplane_Trace* CState_EnemyBiplane_Trace::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Trace* pInstance = new CState_EnemyBiplane_Trace(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Trace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Trace::Free()
{
    __super::Free();
}

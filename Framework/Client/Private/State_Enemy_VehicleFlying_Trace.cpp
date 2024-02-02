#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy_VehicleFlying_Trace.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

CState_Enemy_VehicleFlying_Trace::CState_Enemy_VehicleFlying_Trace(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Trace::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_Enemy_VehicleFlying_Trace::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
    if (nullptr == pPlayer)
        return;
    CCharacter* pCharacter = pPlayer->Get_Character();
    if (nullptr == pCharacter)
        return;

    m_pTarget = pCharacter;
}

void CState_Enemy_VehicleFlying_Trace::Tick_State(_float fTimeDelta)
{
    Vec4 vMyPos = m_pTransformCom->Get_Position();
    Vec4 vPlayerPos = m_pTarget->Get_CharacterTransformCom()->Get_Position();

    // 30 : 추적 시작함. 15 : 공격. // 40 : Run (Temp)
    _float fDistance = (vPlayerPos - vMyPos).Length();
    if (15.f > fDistance)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ATTACK);
        return;
    }
    else if (15.f > fDistance && fDistance <= 40.f)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
        return;
    }
    
    Vec3 vDestLook = XMVector3Normalize(vPlayerPos - vMyPos);
    Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
    Vec3 vNewLook = Vec3::Lerp(vLook, vDestLook, fTimeDelta);

    m_pTransformCom->Rotation_Look(vNewLook);
    m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fTraceSpeed, fTimeDelta);
}

void CState_Enemy_VehicleFlying_Trace::Exit_State()
{

}

CState_Enemy_VehicleFlying_Trace* CState_Enemy_VehicleFlying_Trace::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Trace* pInstance = new CState_Enemy_VehicleFlying_Trace(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Trace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Trace::Free()
{
    __super::Free();
}

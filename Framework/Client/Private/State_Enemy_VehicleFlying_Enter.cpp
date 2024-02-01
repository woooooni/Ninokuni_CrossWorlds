#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_Enemy_VehicleFlying_Enter.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"

#include "UIMinigame_Manager.h"

CState_Enemy_VehicleFlying_Enter::CState_Enemy_VehicleFlying_Enter(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Enter::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;
    
    return S_OK;
}

void CState_Enemy_VehicleFlying_Enter::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Enemy_VehicleFlying_Enter::Tick_State(_float fTimeDelta)
{
    if (m_pVehicle->Get_ObjectTag() != TEXT("Vehicle_EnemyBiplane"))
        return;
    
    CVehicle_Flying* pBiplane = dynamic_cast<CVehicle_Flying*>(m_pVehicle);

    _float4 vDestPos;
    XMStoreFloat4(&vDestPos, pBiplane->Get_RoutePoint(pBiplane->Get_CurIndex()));
    Move(fTimeDelta);

    Vec4 vMyPos = m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

    if (vMyPos.x >= vDestPos.x - 0.1f && vMyPos.x <= vDestPos.x + 0.1f &&
        vMyPos.y >= vDestPos.y - 0.1f && vMyPos.y <= vDestPos.y + 0.1f &&
        vMyPos.z >= vDestPos.z - 0.1f && vMyPos.z <= vDestPos.z + 0.1f) // 내가 지정한 DestPosition과 거의 유사해지면
    {
        if (false == m_bUpdate)
        {
            _int iCurIndex = pBiplane->Get_CurIndex();
            _int iMaxIndex = pBiplane->Get_Routes()->size() - 1;

            if (iCurIndex == iMaxIndex)
            {
                m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE); // Temp
                return;
            }

            pBiplane->Set_CurIndex(iCurIndex++); // 다음 경로를 세팅하고
            m_bUpdate = true; // 불변수 제어
        }
    }
    else
    {
        m_bUpdate = false;
    }
}

void CState_Enemy_VehicleFlying_Enter::Exit_State()
{
    m_bSet = false;
    m_bUpdate = false;
}

void CState_Enemy_VehicleFlying_Enter::Move(_float fTimeDelta)
{
    CVehicle_Flying* pBiplane = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pBiplane)
        return;

    // 현재 인덱스를 받아서 목표 포지션을 설정한다.
    _uint iIndex = pBiplane->Get_CurIndex();
    _float4 vDestPos;
    XMStoreFloat4(&vDestPos, pBiplane->Get_RoutePoint(iIndex));

    // 루트를 짜기 위한 과정임.
    Vec4 vMyPos = m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

    Vec3 vScale = m_pTransformCom->Get_Scale();
    Vec4 vLook = XMVector4Normalize(vDestPos - vMyPos);
    Vec4 vRight = XMVector4Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
    Vec4 vUp = XMVector4Normalize(XMVector3Cross(vLook, vRight));

    if (false == m_bSet)
    {
        m_bSet = true;

        _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

        _matrix DestMatrix = XMMatrixIdentity();
        DestMatrix.r[CTransform::STATE_RIGHT] = vRight * vScale.x;
        DestMatrix.r[CTransform::STATE_UP] = vUp * vScale.y;
        DestMatrix.r[CTransform::STATE_LOOK] = vLook * vScale.z;
        DestMatrix.r[CTransform::STATE_POSITION] = vPosition;

        m_pTransformCom->Set_WorldMatrix(DestMatrix);
    }
    else
    {
        _matrix WorldMatrix = m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix();

        WorldMatrix.r[CTransform::STATE_RIGHT] = vRight * vScale.x;
        WorldMatrix.r[CTransform::STATE_UP] = vUp * vScale.y;
        WorldMatrix.r[CTransform::STATE_LOOK] = vLook * vScale.z;

        m_pTransformCom->Set_WorldMatrix(WorldMatrix);
        m_pTransformCom->Move(vLook, m_fMovingSpeed, fTimeDelta);
    }
}



CState_Enemy_VehicleFlying_Enter* CState_Enemy_VehicleFlying_Enter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Enter* pInstance = new CState_Enemy_VehicleFlying_Enter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Enter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Enter::Free()
{
    __super::Free();
}

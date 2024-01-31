#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy_VehicleFlying_Run.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

CState_Enemy_VehicleFlying_Run::CState_Enemy_VehicleFlying_Run(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_Enemy_VehicleFlying_Run::Enter_State(void* pArg)
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

void CState_Enemy_VehicleFlying_Run::Tick_State(_float fTimeDelta)
{
    if (nullptr == m_pTarget)
        return;

    CVehicle_Flying* pBoto = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pBoto) // 엔지니어 비행기일수도 있음. Desc를 가지고 와서 판단해야함. 일단 Boto
        return;

    if (true == pBoto->Get_Stat().bIsEnemy)
    {
        Vec4 vMyPos = m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
        Vec4 vPlayerPos = m_pTarget->Get_CharacterTransformCom()->Get_Position();

        // 30 : 추적 시작함. 15 : 공격. // 40 : Run (Temp)
        if (30.f > XMVectorGetX(XMVector3Length(vPlayerPos - vMyPos)))
        {
            // 일정 거리 안으로 다가오면 쫓아간다.
 //           // (내가 타겟인 경우는 제외한다. -> 내가 타겟이면 도망가게함)
 //           if (m_pTarget->Get_Target() != m_pVehicle &&
 //               m_pTarget->Get_Target() != m_pVehicle->Get_Rider())
            {
                m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_TRACE);
                return;
            }
        }

        // 이 조건이 걸리는 외의 상황에서는 주어진 Route따라 날아다닌다.
        _float4 vDestPos;
        XMStoreFloat4(&vDestPos, pBoto->Get_RoutePoint(pBoto->Get_CurIndex()));
        Move(fTimeDelta); 

        if (vMyPos.x >= vDestPos.x - 0.1f && vMyPos.x <= vDestPos.x + 0.1f &&
            vMyPos.y >= vDestPos.y - 0.1f && vMyPos.y <= vDestPos.y + 0.1f &&
            vMyPos.z >= vDestPos.z - 0.1f && vMyPos.z <= vDestPos.z + 0.1f) // 내가 지정한 DestPosition과 거의 유사해지면
        {
            if (false == m_bUpdate)
            {
                _int iCurIndex = pBoto->Get_CurIndex();
                _int iMaxIndex = pBoto->Get_Routes()->size() - 1;
                _int iRandom = 0;

                // 업데이트 전이라면
                do {
                    iRandom = GI->RandomInt(0, iMaxIndex);

                } while (iCurIndex == iRandom);
                // 같지 않은 숫자가 나오면 빠져나온다.
                pBoto->Set_CurIndex(iRandom); // 다음 경로를 세팅하고

                m_bUpdate = true; // 불변수 제어
            }
        }
        else
        {
            m_bUpdate = false;
        }
    }
}

void CState_Enemy_VehicleFlying_Run::Exit_State()
{
    m_pTarget = nullptr;
    m_bUpdate = false;
    m_bSet = false;
}

void CState_Enemy_VehicleFlying_Run::Move(_float fTimeDelta)
{
    CVehicle_Flying* pBoto = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pBoto)
        return;

    // 현재 인덱스를 받아서 목표 포지션을 설정한다.
    _uint iIndex = pBoto->Get_CurIndex();
    _float4 vDestPos;
    XMStoreFloat4(&vDestPos, pBoto->Get_RoutePoint(iIndex));

    // 루트를 짜기 위한 과정임.
    Vec4 vMyPos = m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

//    Vec4 vLook = XMVector4Normalize(vPlayerPos - vMyPos);
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

CState_Enemy_VehicleFlying_Run* CState_Enemy_VehicleFlying_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Run* pInstance = new CState_Enemy_VehicleFlying_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Run::Free()
{
    __super::Free();
}

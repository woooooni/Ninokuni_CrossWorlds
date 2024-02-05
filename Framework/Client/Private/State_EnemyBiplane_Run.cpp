#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Run.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Run::CState_EnemyBiplane_Run(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Run::Initialize(const list<wstring>& AnimationList)
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

void CState_EnemyBiplane_Run::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    m_bUpdate = false;

    
}

void CState_EnemyBiplane_Run::Tick_State(_float fTimeDelta)
{
    if (nullptr == m_pTarget)
        return;

    CVehicle_Flying* pFlying = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pFlying) // 엔지니어 비행기일수도 있음. Desc를 가지고 와서 판단해야함. 일단 Boto
        return;

    Vec4 vMyPos = m_pTransformCom->Get_Position();
    Vec4 vPlayerPos = m_pTarget->Get_CharacterTransformCom()->Get_Position();

    Vec3 vDir = vPlayerPos - vMyPos;
    // 30 : 추적 시작함. 15 : 공격. // 40 : Run (Temp)
    if (30.f > vDir.Length())
    {
        // 추적 시작
        return;
    }

    // 이 조건이 걸리는 외의 상황에서는 주어진 Route따라 날아다닌다.

    Vec4 vDestPos = pFlying->Get_RoutePoint(pFlying->Get_CurIndex());
    Move(fTimeDelta);

    _float fDistanceDest = (vDestPos - Vec4(m_pTransformCom->Get_Position())).Length();
    if (0.1f > fDistanceDest) // 내가 지정한 DestPosition과 거의 유사해지면
    {
        if (false == m_bUpdate)
        {
            _int iCurIndex = pFlying->Get_CurIndex();
            _int iMaxIndex = pFlying->Get_Routes()->size() - 1;
            _int iRandom = 0;

            // 업데이트 전이라면
            do {
                iRandom = GI->RandomInt(0, iMaxIndex);
            } while (iCurIndex == iRandom);
            // 같지 않은 숫자가 나오면 빠져나온다.
            pFlying->Set_CurIndex(iRandom); // 다음 경로를 세팅하고

            m_bUpdate = true; // 불변수 제어
        }
    }
    else
    {
        m_bUpdate = false;
    }
}

void CState_EnemyBiplane_Run::Exit_State()
{
    m_bUpdate = false;
}

void CState_EnemyBiplane_Run::Move(_float fTimeDelta)
{
    CVehicle_Flying* pBoto = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pBoto)
        return;

    // 현재 인덱스를 받아서 목표 포지션을 설정한다.
    _uint iIndex = pBoto->Get_CurIndex();
    Vec4 vDestPos = pBoto->Get_RoutePoint(iIndex);

    // 루트를 짜기 위한 과정임.
    Vec4 vMyPos = m_pTransformCom->Get_Position();

    if (0.1f < Vec3(vDestPos - vMyPos).Length())
    {
        Vec3 vDestLook = XMVector3Normalize(vDestPos - vMyPos);
        Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());

        Vec3 vLerpLook = Vec3::Lerp(vLook, vDestLook, 5.f * fTimeDelta);

        m_pTransformCom->Rotation_Look(XMVector3Normalize(vLerpLook));
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMovingSpeed, fTimeDelta);
    }
}

CState_EnemyBiplane_Run* CState_EnemyBiplane_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Run* pInstance = new CState_EnemyBiplane_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Run::Free()
{
    __super::Free();
}

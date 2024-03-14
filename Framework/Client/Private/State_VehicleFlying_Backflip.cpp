#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_VehicleFlying_Backflip.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"
#include "Vehicle_Flying_Biplane.h"

#include "Trail.h"

#include "UIMinigame_Manager.h"

CState_VehicleFlying_Backflip::CState_VehicleFlying_Backflip(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Backflip::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_VehicleFlying_Backflip::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
    {
        /* 수정시 카메라 담당한테 이야기 해주세요 */
        pFollowCam->Reset_WideView_To_DefaultView(true);
        pFollowCam->Set_Default_Position();
        pFollowCam->Set_DampingBackLimitRad(XMConvertToRadians(90.f));
        pFollowCam->Set_CanInput(true);
        pFollowCam->Set_MinMaxLimitY(0.7f, 2.1f);
    }

    CVehicle_Flying_Biplane* pBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pVehicle);
    if (nullptr != pBiplane)
    {
        pBiplane->Stop_Trail();

        for (_uint i = 0; i < CVehicle_Flying_Biplane::BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
        {
            CTrail* pTrail = pBiplane->Get_Trail(CVehicle_Flying_Biplane::BIPLANE_TRAIL(i));
            if (nullptr == pTrail)
                continue;

            CTrail::TRAIL_DESC TrailDesc = pTrail->Get_TrailDesc();
            TrailDesc.vDistortion = Vec2(0.f, 0.f);
            pTrail->Set_TrailDesc(TrailDesc);
        }

        CGameObject* pRider = pBiplane->Get_Rider();
        if (nullptr != pRider)
        {
            pRider->Get_Component_StateMachine()->Change_State(CCharacter::STATE::FLYING_STAND);
        }
        
    }

    GI->Stop_Sound(CHANNELID::SOUND_VEHICLE);
    GI->Play_Sound(TEXT("sp_ie_st07_pool_core_smash_hit_01.wav"), CHANNELID::SOUND_VEHICLE, 0.4f);
    
    m_bFinished = false;
    m_fTimeAcc = 0.f;
    m_fAccumulatedRotation = 0.f;
}

void CState_VehicleFlying_Backflip::Tick_State(_float fTimeDelta)
{
    if (false == m_bFinished)
    {
        m_fTimeAcc += fTimeDelta;

        if (1.f < m_fTimeAcc)
        {
            m_bFinished = true;
            m_pStateMachineCom->Change_State(CVehicle::VEHICLE_IDLE);
            return;
        }

        _vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
        _vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
        _float fDot = XMVectorGetX(XMVector3Dot(vRight, vLook));
        m_pTransformCom->Rotation_Acc(vRight, acos(fDot) * fTimeDelta * 5.f);
    }

    if (true == GI->Mouse_Down(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(true);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(true);
    }

    if (true == GI->Mouse_Up(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(false);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(false);
    }

    if (KEY_HOLD(KEY::RBTN))
    {
        if (false == CUIMinigame_Manager::GetInstance()->Is_AimActive())
            CUIMinigame_Manager::GetInstance()->Set_GrandprixAimActive(true);
    }

    if (KEY_NONE(KEY::RBTN))
    {
        CUIMinigame_Manager::GetInstance()->Set_GrandprixAimActive(false);
    }
}

void CState_VehicleFlying_Backflip::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);

    m_bFinished = false;
    m_fTimeAcc = 0.f;
    m_fAccumulatedRotation = 0.f;
    m_pRigidBodyCom->Set_Velocity(Vec3(0.f, 0.f, 0.f));
}

CState_VehicleFlying_Backflip* CState_VehicleFlying_Backflip::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Backflip* pInstance = new CState_VehicleFlying_Backflip(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Backflip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Backflip::Free()
{
    __super::Free();
}

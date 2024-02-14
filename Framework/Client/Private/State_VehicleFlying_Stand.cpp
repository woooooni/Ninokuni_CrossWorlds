#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_VehicleFlying_Stand.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"

#include "UIMinigame_Manager.h"
#include "Character_Biplane_Bullet.h"
#include "Pool.h"

#include "Vehicle_Flying_Biplane.h"
#include "Trail.h"

CState_VehicleFlying_Stand::CState_VehicleFlying_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pVehicle_Flying_Biplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pStateMachineCom->Get_Owner());
    m_pFollowCamera = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

    if (nullptr == m_pVehicle || nullptr == m_pVehicle_Flying_Biplane || nullptr == m_pFollowCamera)
        return E_FAIL;
    
    return S_OK;
}

void CState_VehicleFlying_Stand::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    m_pFollowCamera->Set_DampingBackLimitRad(XMConvertToRadians(90.f));
    m_pFollowCamera->Set_CanInput(true);
    m_pFollowCamera->Set_MinMaxLimitY(0.7f, 2.1f);


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
    }

    CGameObject* pRider = m_pVehicle_Flying_Biplane->Get_Rider();
    if (nullptr != pRider)
    {
        pRider->Get_Component_StateMachine()->Change_State(CCharacter::STATE::FLYING_STAND);
    }
    
}

void CState_VehicleFlying_Stand::Tick_State(_float fTimeDelta)
{
    if (true == m_bEngineer && (GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER).size() <= 1))
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
        return;
    }

    _bool bMove = false;

    // Skills
    if (KEY_TAP(KEY::NUM_1))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_MUCKCLOUD);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_MUCKCLOUD);
//        GI->Stop_Sound(CHANNELID::SOUND_UI);
//        GI->Play_Sound(TEXT("UI_Fx_Comm_Start_Skill_Imajinn_2.mp3"), CHANNELID::SOUND_UI,
//            GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
        return;
    }
//    if (KEY_TAP(KEY::NUM_2))
//    {
//        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_RUSH);
//
//        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_TEMP2);
//        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_TEMP2);
//        GI->Stop_Sound(CHANNELID::SOUND_UI);
//        GI->Play_Sound(TEXT("UI_Fx_Comm_Start_Skill_Imajinn_2.mp3"), CHANNELID::SOUND_UI,
//        GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
//        return;
//    }
    if (KEY_TAP(KEY::NUM_3))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_GUIDED_MISSILE);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_GUIDED_MISSILE);
//        GI->Stop_Sound(CHANNELID::SOUND_UI);
//        GI->Play_Sound(TEXT("UI_Fx_Comm_Start_Skill_Imajinn_2.mp3"), CHANNELID::SOUND_UI,
//            GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
        return;
    }
    if (KEY_TAP(KEY::R))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_BURST);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_BURST);
        GI->Stop_Sound(CHANNELID::SOUND_UI);
        GI->Play_Sound(TEXT("UI_Fx_Comm_Start_Skill_Imajinn_2.mp3"), CHANNELID::SOUND_UI,
            GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
        return;
    }

    if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
    {
        if (false == m_bInitVelocity)
        {
            m_pRigidBodyCom->Set_Velocity(Vec3(0.f, 0.f, 0.f));
            m_pRigidBodyCom->Set_Ground(false);
            m_pRigidBodyCom->Set_Use_Gravity(false);
            m_bInitVelocity = true;
        }

        if (false == m_bShoot)
        {
            m_fTimeAcc += fTimeDelta;
            
            if (0.1f < m_fTimeAcc)
            {
                m_bShoot = true;
                m_fTimeAcc = 0.f;
            }
        }

        if (KEY_HOLD(KEY::Q))
        {
            _vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
            _vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
            _float fDot = XMVectorGetX(XMVector3Dot(vRight, vLook));

            m_pTransformCom->Rotation_Acc(vRight, acos(fDot) * fTimeDelta * -1.f);
        }

        if (KEY_HOLD(KEY::E))
        {
            _vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
            _vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
            _float fDot = XMVectorGetX(XMVector3Dot(vRight, vLook));

            m_pTransformCom->Rotation_Acc(vRight, acos(fDot) * fTimeDelta);
        }

        if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        {
            m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
            return;
        }

        if (KEY_HOLD(KEY::LBTN))
        {
            if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH)
            {
                if (true == m_bShoot)
                    Shoot();
            }
        }
    }
    else
    {
        m_pRigidBodyCom->Set_Ground(false);

        if (KEY_TAP(KEY::SPACE))
        {
            Vec3 vVelocity = m_pRigidBodyCom->Get_Velocity();
            vVelocity.y = 0.f;

            _float fDecisionSpeed = 10.f;

            _float fAddSpeed = 0.f;
#ifdef _DEBUG
            fAddSpeed = 400.f;
#endif // _DEBUG
#ifndef _DEBUG
            fAddSpeed = 800.f;
#endif // !_DEBUG
            if (vVelocity.Length() >= fDecisionSpeed) // 20.f -> 15.f ¼öÁ¤
            {
                m_pRigidBodyCom->Set_Use_Gravity(false);
                Vec3 vVelocityDir = XMVector3Normalize(m_pTransformCom->Get_Look());
                vVelocityDir.y = 0.8f;
                m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(vVelocityDir), fAddSpeed * fTimeDelta, false);
            }
            else
            {
                m_pRigidBodyCom->Set_Use_Gravity(true);
                Vec3 vVelocityDir = m_pTransformCom->Get_Look();
                vVelocityDir.y = 0.f;

                m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(vVelocityDir), fAddSpeed * fTimeDelta, false);
            }
        }
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

//    if (KEY_HOLD(KEY::Q))
//    {
//        bMove = true;
//
//        _matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
//        _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
//        //_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];
//        _vector vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
//
//        vRight = XMVector3Normalize(vRight);
//        //vCamLook = XMVector3Normalize(vCamLook);
//        vUp = XMVector3Normalize(vUp);
//
//        //_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;
//        _float fRadian = XMVectorGetX(XMVector3Dot(vRight, vUp)) * 10.f * fTimeDelta;
//
//        m_pTransformCom->Rotation_Acc(vUp, fRadian);
//        //m_pTransformCom->Move(vUp, m_pVehicle->Get_Speed(), fTimeDelta);
//    }

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

void CState_VehicleFlying_Stand::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    
    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);

    m_bShoot = false;
    m_fTimeAcc = 0.f;
}

void CState_VehicleFlying_Stand::Shoot()
{
    CCharacter_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.pOwner = dynamic_cast<CVehicle_Flying*>(m_pVehicle);

    // Left Side Bullet

    CGameObject* pLeftBullet = CPool<CCharacter_Biplane_Bullet>::Get_Obj();

    if (nullptr == pLeftBullet)
        pLeftBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

    if (nullptr == pLeftBullet)
        return;

    CTransform* pLeftTransform = pLeftBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vLeftScale = pLeftTransform->Get_Scale();

    pLeftTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pLeftTransform->Set_Scale(vLeftScale);

    _matrix MatLeft = m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix();
    Vec4 vLeftStartPos = MatLeft.r[CTransform::STATE_POSITION];

    //vLeftStartPos += XMVector3Normalize(pLeftTransform->Get_Look()) * 0.5f;
    vLeftStartPos += XMVector3Normalize(m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Look()) * 0.5f;
    pLeftTransform->Set_State(CTransform::STATE_POSITION, vLeftStartPos);

    if (nullptr != m_pVehicle_Flying_Biplane->Get_Target())
    {
        CTransform* pTargetTransform = m_pVehicle_Flying_Biplane->Get_Target()->Get_Component_Transform();
        if (nullptr != pTargetTransform)
        {
            Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() + XMVectorSet(0.f, 1.5f, 0.f, 0.f) - pLeftTransform->Get_Position());
            pLeftTransform->Rotation_Look(vDir);
        }
    }

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pLeftBullet)))
        MSG_BOX("Generate Bullet Failed.");


    // Right Side Bullet

    CGameObject* pRightBullet = CPool<CCharacter_Biplane_Bullet>::Get_Obj();

    if (nullptr == pRightBullet)
        pRightBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

    if (nullptr == pRightBullet)
        return;

    CTransform* pRightTransform = pRightBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vRightScale = pRightTransform->Get_Scale();
    pRightTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pRightTransform->Set_Scale(vRightScale);

    _matrix MatRight = m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix();
    Vec4 vRightStartPos = MatRight.r[CTransform::STATE_POSITION];
    vRightStartPos += XMVector3Normalize(m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Look()) * 0.5f;
    pRightTransform->Set_State(CTransform::STATE_POSITION, vRightStartPos);

    if (nullptr != m_pVehicle_Flying_Biplane->Get_Target())
    {
        CTransform* pTargetTransform = m_pVehicle_Flying_Biplane->Get_Target()->Get_Component_Transform();
        if (nullptr != pTargetTransform)
        {
            Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() + XMVectorSet(0.f, 1.5f, 0.f, 0.f) - pRightTransform->Get_Position());
            pRightTransform->Rotation_Look(vDir);
        }
    }

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pRightBullet)))
        MSG_BOX("Generate Bullet Failed.");

    GI->Play_Sound(L"Pc_Rifle_Shot_Combo_Basic_1.mp3", CHANNELID::SOUND_ATTACK, 0.5f, true);

    m_bShoot = false;
}

CState_VehicleFlying_Stand* CState_VehicleFlying_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Stand* pInstance = new CState_VehicleFlying_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Stand::Free()
{
    __super::Free();
}

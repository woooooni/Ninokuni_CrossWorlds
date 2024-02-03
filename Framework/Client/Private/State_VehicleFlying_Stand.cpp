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

CState_VehicleFlying_Stand::CState_VehicleFlying_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;
    
    return S_OK;
}

void CState_VehicleFlying_Stand::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(false);
}

void CState_VehicleFlying_Stand::Tick_State(_float fTimeDelta)
{
    _bool bMove = false;

    // Skills
    if (KEY_TAP(KEY::NUM_1))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_MUCKCLOUD);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_MUCKCLOUD);
        return;
    }
//    if (KEY_TAP(KEY::NUM_2))
//    {
//        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_RUSH);
//
//        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_TEMP2);
//        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_TEMP2);
//        return;
//    }
    if (KEY_TAP(KEY::NUM_3))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_GUIDED_MISSILE);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_GUIDED_MISSILE);
        return;
    }
    if (KEY_TAP(KEY::R))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_BURST);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_BURST);
        return;
    }

    if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
    {
        if (false == m_bShoot)
        {
            m_fTimeAcc += fTimeDelta;
            
            if (0.5f < m_fTimeAcc)
            {
                m_bShoot = true;
                m_fTimeAcc = 0.f;
            }
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
        if (KEY_TAP(KEY::SPACE))
        {
            Vec3 vVelocity = m_pRigidBodyCom->Get_Velocity();
            vVelocity.y = 0.f;

            if (vVelocity.Length() >= 15.f) // 20.f -> 15.f 수정
            {
                Vec3 vVelocityDir = XMVector3Normalize(m_pTransformCom->Get_Look());
                vVelocityDir.y = 0.8f;
                m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(vVelocityDir), 5.f, false);
                m_pRigidBodyCom->Set_Use_Gravity(false);
            }
            else
            {
                Vec3 vVelocityDir = m_pTransformCom->Get_Look();
                vVelocityDir.y = 0.f;
                m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(vVelocityDir), 200.f * fTimeDelta, false); // 200.f -> 100.f로 수정
                m_pRigidBodyCom->Set_Use_Gravity(true);
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

    if (KEY_HOLD(KEY::Q))
    {
        bMove = true;

        _matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
        _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
        _vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

        vRight = XMVector3Normalize(vRight);
        vCamLook = XMVector3Normalize(vCamLook);

        _float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;

        m_pTransformCom->Rotation_Acc(vRight, fRadian);
        m_pTransformCom->Move(vCamLook, m_pVehicle->Get_Speed(), fTimeDelta);
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
    CGameObject* pLeftBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
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

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pLeftBullet)))
        MSG_BOX("Generate Bullet Failed.");


    // Right Side Bullet
    CGameObject* pRightBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
    if (nullptr == pRightBullet)
        return;

    CTransform* pRightTransform = pRightBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vRightScale = pRightTransform->Get_Scale();
    pRightTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pRightTransform->Set_Scale(vRightScale);

    _matrix MatRight = m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix();
    Vec4 vRightStartPos = MatRight.r[CTransform::STATE_POSITION];
   // vRightStartPos += XMVector3Normalize(pRightTransform->Get_Look()) * 0.5f;
    vRightStartPos += XMVector3Normalize(m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Look()) * 0.5f;
    pRightTransform->Set_State(CTransform::STATE_POSITION, vRightStartPos);

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pRightBullet)))
        MSG_BOX("Generate Bullet Failed.");

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

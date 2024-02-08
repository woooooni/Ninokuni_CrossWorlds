#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_VehicleFlying_BossStage_Stand.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"

#include "UIMinigame_Manager.h"
#include "Character_Biplane_Bullet.h"
#include "Pool.h"

#include "Vehicle_Flying_Biplane.h"
#include "Trail.h"

CState_VehicleFlying_BossStage_Stand::CState_VehicleFlying_BossStage_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_BossStage_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pVehicle_Flying_Biplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pStateMachineCom->Get_Owner());
    m_pFollowCamera = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

    if (nullptr == m_pVehicle || nullptr == m_pVehicle_Flying_Biplane || nullptr == m_pFollowCamera)
        return E_FAIL;

    for (auto& pCollider : m_pVehicle_Flying_Biplane->Get_Collider(CCollider::DETECTION_TYPE::BOUNDARY))
        pCollider->Set_Radius(100.f);

    
    return S_OK;
}

void CState_VehicleFlying_BossStage_Stand::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

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
    
}

void CState_VehicleFlying_BossStage_Stand::Tick_State(_float fTimeDelta)
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
        return;
    }

    if (KEY_TAP(KEY::NUM_2))
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_RUSH);

        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_RUSH);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_RUSH);
        return;
    }

    if (KEY_TAP(KEY::NUM_3))
    {
        CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_GUIDED_MISSILE);
        CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_GUIDED_MISSILE);
        return;
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

    if (KEY_HOLD(KEY::W))
    {
        m_pVehicle_Flying_Biplane->Look_For_Target();
        if (nullptr != m_pVehicle_Flying_Biplane->Get_Target())
        {
            Vec4 vTargetPos = m_pVehicle_Flying_Biplane->Get_Target()->Get_Component_Transform()->Get_Position();
            Vec3 vDir = vTargetPos - Vec4(m_pTransformCom->Get_Position());
            if (vDir.Length() > 2.f)
                m_pTransformCom->Move(1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 5.f, fTimeDelta);
        }
        else        
            m_pTransformCom->Move(1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 5.f, fTimeDelta);
    }

    if (KEY_HOLD(KEY::A))
    {
        m_pVehicle_Flying_Biplane->Look_For_Target();
        m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 5.f, fTimeDelta);
    }

    if (KEY_HOLD(KEY::S))
    {
        m_pVehicle_Flying_Biplane->Look_For_Target();
        m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 5.f, fTimeDelta);
    }

    if (KEY_HOLD(KEY::D))
    {
        m_pVehicle_Flying_Biplane->Look_For_Target();
        m_pTransformCom->Move(1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 5.f, fTimeDelta);
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

void CState_VehicleFlying_BossStage_Stand::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    
    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);

    m_bShoot = false;
    m_fTimeAcc = 0.f;
}

void CState_VehicleFlying_BossStage_Stand::Shoot()
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
            Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f) - pLeftTransform->Get_Position());
            pLeftTransform->Rotation_Look(vDir);
        }
    }

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pLeftBullet)))
        MSG_BOX("Generate Bullet Failed.");


    // Right Side Bullet

    CGameObject* pRightBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

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
            Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f) - pRightTransform->Get_Position());
            pRightTransform->Rotation_Look(vDir);
        }
    }

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pRightBullet)))
        MSG_BOX("Generate Bullet Failed.");

    m_bShoot = false;
}

CState_VehicleFlying_BossStage_Stand* CState_VehicleFlying_BossStage_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_BossStage_Stand* pInstance = new CState_VehicleFlying_BossStage_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_BossStage_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_BossStage_Stand::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Stand.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Riding_Manager.h"
#include "Vehicle_Flying_EnemyBiplane.h"
#include "Vehicle_Flying_Biplane.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Enemy_Biplane_Bullet.h"
#include "Enemy_Biplane_BulletBall.h"
#include "Enemy_GuidedMissile.h"

#include "Pool.h"


CState_EnemyBiplane_Stand::CState_EnemyBiplane_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());
    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
    
   

    
    m_pFollowCamera = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr == m_pFollowCamera)
        return E_FAIL;

    

    

    return S_OK;
}

void CState_EnemyBiplane_Stand::Enter_State(void* pArg)
{
    m_pEngineerPlane->Set_Infinite(false, 0.f);


    if (true == m_bFirstEnter)
    {
        // TODO : 플레이어 포지션 변경.
        CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform()->Set_Position(Vec4(-0.65f, 51.4f, 210.f, 1.f));
        CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component<CPhysX_Controller>(L"Com_Controller")->Set_EnterLevel_Position(Vec4(-0.65f, 51.4f, 210.f, 1.f));

        // TODO : 내 포지션 변경.
        m_pTransformCom->Set_Position(Vec4(-0.65f, 111.44f, 288.f, 1.f));
        m_pEngineerPlane->Get_Component<CPhysX_Controller>(L"Com_Controller")->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
        m_vInitialPositon = m_pTransformCom->Get_Position();


        // TODO:: Look 변경,
        Vec3 vTargetDir = m_pTransformCom->Get_Position() - CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform()->Get_Position();

        CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform()->Rotation_Look(XMVector3Normalize(vTargetDir));
        CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_StateMachine()->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_BOSS_IDLE);
        m_pTransformCom->Rotation_Look(-1.f * XMVector3Normalize(vTargetDir));

        // TODO : Camera Target
        m_pFollowCamera->Start_LockOn(m_pEngineerPlane, Vec4(0.f, -1.f, -20.f, 0.f), Vec4(0.f, 1.f, 0.f, 0.f));

        // TODO : 페이드.
        CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);

        m_bFirstEnter = false;
    }

    for (_int i = 0; i < SHOOT_STATE::SHOOT_STATE_END; ++i)
        m_iShootCounts[i] = 0;

    return;
}

void CState_EnemyBiplane_Stand::Tick_State(_float fTimeDelta)
{
    if (nullptr == m_pTarget)
        return;

    if (true == m_bCoolDown)
    {
        m_fAccCoolDown += fTimeDelta;
        if (m_fAccCoolDown >= m_fCoolDownTime)
        {
            m_bCoolDown = false;
            m_fAccCoolDown = 0.f;
        }
    }
    else
    {
        m_fAccShootTime += fTimeDelta;

        Vec3 vDir = m_pTarget->Get_CharacterTransformCom()->Get_Position() - m_pTransformCom->Get_Position();
        Vec3 vLook = Vec3::Lerp(XMVector3Normalize(m_pTransformCom->Get_Look()), XMVector3Normalize(vDir), fTimeDelta);

        m_pTransformCom->Rotation_Look(vLook);

        if (m_fAccShootTime >= m_fShootTime)
        {
            m_fAccShootTime = 0.f;
            switch (m_eShootState)
            {
            case Client::CState_EnemyBiplane_Stand::NORMAL:
                Shoot_Normal(fTimeDelta);
                break;
            case Client::CState_EnemyBiplane_Stand::PATTERN_0:
                Shoot_Pattern_0(fTimeDelta);
                break;
            case Client::CState_EnemyBiplane_Stand::PATTERN_1:
                Shoot_Pattern_1(fTimeDelta);
                break;
            case Client::CState_EnemyBiplane_Stand::PATTERN_2:
                Shoot_Pattern_2(fTimeDelta);
                break;
            case Client::CState_EnemyBiplane_Stand::PATTERN_3:
                Shoot_Pattern_3(fTimeDelta);
                break;
            default:
                break;
            }
        }
    }

    if ((nullptr != m_pFollowCamera->Get_LookAtObj()) && (m_pFollowCamera->Get_LookAtObj()->Get_ObjectType() != OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE))
    {
        m_pFollowCamera->Set_TargetObj(m_pTarget);
        m_pFollowCamera->Set_LookAtObj(m_pEngineerPlane);
    }
}

void CState_EnemyBiplane_Stand::Exit_State()
{
    for (_int i = 0; i < SHOOT_STATE::SHOOT_STATE_END; ++i)    
        m_iShootCounts[i] = 0;
}

void CState_EnemyBiplane_Stand::Shoot_Normal(_float fTimeDelta)
{
    m_fShootTime = 0.2f;
    if (m_iShootCounts[SHOOT_STATE::NORMAL] > 10)
    {
        // m_eShootState = SHOOT_STATE(GI->RandomInt(SHOOT_STATE::PATTERN_0, SHOOT_STATE::PATTERN_2));
        m_iShootCounts[SHOOT_STATE::NORMAL] = 0;
        m_eShootState = SHOOT_STATE(GI->RandomInt(SHOOT_STATE::PATTERN_0, SHOOT_STATE::PATTERN_3));
        return;
    }

    CEnemy_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.pOwner = m_pEngineerPlane;
    // Left Side Bullet

    CEnemy_Biplane_Bullet* pLeftBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

    if (nullptr == pLeftBullet)
        pLeftBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

    if (nullptr == pLeftBullet)
        return;

    Vec3 vTargetDir = {}; 

    CTransform* pLeftTransform = pLeftBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vLeftScale = pLeftTransform->Get_Scale();

    pLeftTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pLeftTransform->Set_Scale(vLeftScale);

    _matrix MatLeft = m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix();
    Vec4 vLeftStartPos = MatLeft.r[CTransform::STATE_POSITION];

    //vLeftStartPos += XMVector3Normalize(pLeftTransform->Get_Look()) * 0.5f;
    vLeftStartPos += XMVector3Normalize(m_pTransformCom->Get_Look()) * 0.5f;
    pLeftTransform->Set_State(CTransform::STATE_POSITION, vLeftStartPos);

    vTargetDir = CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform()->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f) - pLeftTransform->Get_Position();

    pLeftTransform->Rotation_Look(XMVector3Normalize(vTargetDir));
    pLeftBullet->Set_Speed(40.f);

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pLeftBullet)))
        MSG_BOX("Generate Bullet Failed.");


    // Right Side Bullet
    CEnemy_Biplane_Bullet* pRightBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

    if (nullptr == pRightBullet)
        pRightBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

    if (nullptr == pRightBullet)
        return;

    CTransform* pRightTransform = pRightBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vRightScale = pRightTransform->Get_Scale();
    pRightTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pRightTransform->Set_Scale(vRightScale);

    _matrix MatRight = m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix();
    Vec4 vRightStartPos = MatRight.r[CTransform::STATE_POSITION];
    vRightStartPos += XMVector3Normalize(m_pTransformCom->Get_Look()) * 0.5f;
    pRightTransform->Set_State(CTransform::STATE_POSITION, vRightStartPos);

    vTargetDir = CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform()->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f) - pRightTransform->Get_Position();

    pRightTransform->Rotation_Look(XMVector3Normalize(vTargetDir));
    pRightBullet->Set_Speed(40.f);

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pRightBullet)))
        MSG_BOX("Generate Bullet Failed.");


    m_iShootCounts[SHOOT_STATE::NORMAL] += 1;
}

void CState_EnemyBiplane_Stand::Shoot_Pattern_0(_float fTimeDelta)
{
    m_fShootTime = 0.1f;

    if (m_iShootCounts[SHOOT_STATE::PATTERN_0] >= 360)
    {
        m_iShootCounts[SHOOT_STATE::PATTERN_0] = 0;
        m_eShootState = SHOOT_STATE::PATTERN_1;
        m_fShootTime = 1.f;
        return;
    }

    for (_int i = -2; i <= 2; ++i)
    {
        CEnemy_Biplane_Bullet* pBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

        CEnemy_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = m_pEngineerPlane;

        if (nullptr == pBullet)
            pBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

        CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
        Vec3 vScale = pBulletTransform->Get_Scale();

        pBulletTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
        pBulletTransform->Set_Scale(vScale);

        pBulletTransform->Set_Position(pBulletTransform->Get_Position() + XMVectorSet(0.f, 1.5f + i * 4.f, 0.f, 0.f));

        pBulletTransform->Rotation_Acc(XMVector3Normalize(pBulletTransform->Get_Up()), XMConvertToRadians(m_iShootCounts[SHOOT_STATE::PATTERN_0]) * 3.f);
        pBullet->Set_Speed(40.f);

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pBullet)))
            MSG_BOX("Generate Bullet Failed.");
    }
    

    m_iShootCounts[SHOOT_STATE::PATTERN_0] += 1;
    
}

void CState_EnemyBiplane_Stand::Shoot_Pattern_1(_float fTimeDelta)
{
    m_fShootTime = 0.1f;

    if (m_iShootCounts[SHOOT_STATE::PATTERN_1] >= 360)
    {
        m_iShootCounts[SHOOT_STATE::PATTERN_1] = 0;
        m_eShootState = SHOOT_STATE::NORMAL;
        m_fShootTime = 1.f;
        return;
    }

    for (_int i = -2; i <= 2; ++i)
    {
        CEnemy_Biplane_Bullet* pBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

        CEnemy_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = m_pEngineerPlane;

        if (nullptr == pBullet)
            pBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

        CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
        Vec3 vScale = pBulletTransform->Get_Scale();

        pBulletTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
        pBulletTransform->Set_Scale(vScale);

        pBulletTransform->Set_Position(pBulletTransform->Get_Position() + XMVectorSet(i * 4.f, 1.5f, 0.f, 0.f));

        pBulletTransform->Rotation_Acc(XMVector3Normalize(pBulletTransform->Get_Right()), XMConvertToRadians(m_iShootCounts[SHOOT_STATE::PATTERN_1]) * 3.f);
        pBullet->Set_Speed(40.f);

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pBullet)))
            MSG_BOX("Generate Bullet Failed.");
    }


    m_iShootCounts[SHOOT_STATE::PATTERN_1] += 1;
}

void CState_EnemyBiplane_Stand::Shoot_Pattern_2(_float fTimeDelta)
{
    m_fShootTime = 1.f;

    if (m_iShootCounts[SHOOT_STATE::PATTERN_2] >= 4)
    {
        m_iShootCounts[SHOOT_STATE::PATTERN_2] = 0;
        m_eShootState = SHOOT_STATE::NORMAL;
        return;
    }

    // 사선 모양으로 (/, \) 총알을 발사.
    for (_int i = -20; i <= 20; ++i)
    {
        CEnemy_Biplane_Bullet* pBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

        CEnemy_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = m_pEngineerPlane;

        if (nullptr == pBullet)
            pBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

        CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
        Vec3 vScale = pBulletTransform->Get_Scale();

        pBulletTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
        pBulletTransform->Set_Scale(vScale);

        _int iOffset = (m_iShootCounts[SHOOT_STATE::PATTERN_2] % 2 == 1) ? -1 : 1;


        Vec4 vOffsetPosition = m_pTransformCom->Get_RelativeOffset(Vec4(i, -1.f * iOffset * i * 0.5f + 1.f, 0.f, 1.f));
        pBulletTransform->Set_Position(vOffsetPosition + Vec4(m_pTransformCom->Get_Position()));

        pBullet->Set_Speed(40.f);

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pBullet)))
            MSG_BOX("Generate Bullet Failed.");
    }

    m_iShootCounts[SHOOT_STATE::PATTERN_2] += 1;
    

}

void CState_EnemyBiplane_Stand::Shoot_Pattern_3(_float fTimeDelta)
{
    m_fShootTime = 0.5f;
    if (m_iShootCounts[SHOOT_STATE::PATTERN_3] >= 50)
    {
        m_iShootCounts[SHOOT_STATE::PATTERN_3] = 0;
        m_eShootState = SHOOT_STATE::NORMAL;
        return;
    }

    _int iBulletCount = GI->RandomInt(1, 4);
    for (_int i = 0; i < iBulletCount; ++i)
    {
        CEnemy_Biplane_Bullet* pBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

        CEnemy_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = m_pEngineerPlane;

        if (nullptr == pBullet)
            pBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

        CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
        Vec3 vScale = pBulletTransform->Get_Scale();

        pBulletTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
        pBulletTransform->Set_Scale(vScale);

        Vec4 vOffset = pBulletTransform->Get_RelativeOffset(Vec4(GI->RandomInt(-5, 5), i - (i / 2), -1.f, 1.f));
        Vec4 vFinalPos = Vec4(pBulletTransform->Get_Position()) + vOffset;
        pBulletTransform->Set_Position(vFinalPos);

        pBullet->Set_Speed(40.f);

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pBullet)))
            MSG_BOX("Generate Bullet Failed.");
    }

    m_iShootCounts[SHOOT_STATE::PATTERN_3] += 1;
}


CState_EnemyBiplane_Stand* CState_EnemyBiplane_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Stand* pInstance = new CState_EnemyBiplane_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Stand::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Skill_1.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Enemy_Biplane_BulletBall.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Skill_1::CState_EnemyBiplane_Skill_1(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Skill_1::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    m_iShootBallCount = -2;
    return S_OK;
}

void CState_EnemyBiplane_Skill_1::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_EnemyBiplane_Skill_1::Tick_State(_float fTimeDelta)
{
    for (_int i = m_iShootBallCount; i <= 2; ++i)
    {
        Shoot_Bullet_Ball();
        m_iShootBallCount++;
    }
        

    m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
    return;
}

void CState_EnemyBiplane_Skill_1::Exit_State()
{
    m_iShootBallCount = -2;
}


void CState_EnemyBiplane_Skill_1::Shoot_Bullet_Ball()
{
    CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.bPool = false;
    ProjectileDesc.pOwner = m_pEngineerPlane;

    CGameObject* pProjectile = nullptr;
    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Enemy_Biplane_BulletBall", &ProjectileDesc, &pProjectile)))
    {
        MSG_BOX("AddGameObject_Failed. : CState_EnemyBiplane_Skill_1::Shoot_Bullet_Ball()");
        return;
    }

    if (nullptr == pProjectile)
    {
        MSG_BOX("pProjectile is Null. : CState_EnemyBiplane_Skill_1::Shoot_Bullet_Ball()");
        return;
    }

    CEnemy_Biplane_BulletBall* pBulletBall = dynamic_cast<CEnemy_Biplane_BulletBall*>(pProjectile);

    if (nullptr == pBulletBall)
    {
        MSG_BOX("pProjectile Cast Failed. : CState_EnemyBiplane_Skill_1::Shoot_Bullet_Ball()");
        return;
    }

    CTransform* pProjectileTransform = pProjectile->Get_Component_Transform();
    if (nullptr == pProjectileTransform)
    {
        MSG_BOX("pProjectileTransform is Null. : CState_EnemyBiplane_Skill_1::Shoot_Bullet_Ball()");
        return;
    }

    Vec3 vScale = pProjectileTransform->Get_Scale();
    pProjectileTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pProjectileTransform->Set_Scale(vScale);

    pBulletBall->Set_OffsetPosition(Vec4(m_iShootBallCount, 1.5f, 2.f, 1.f));
}

CState_EnemyBiplane_Skill_1* CState_EnemyBiplane_Skill_1::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Skill_1* pInstance = new CState_EnemyBiplane_Skill_1(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Skill_1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Skill_1::Free()
{
    __super::Free();
}

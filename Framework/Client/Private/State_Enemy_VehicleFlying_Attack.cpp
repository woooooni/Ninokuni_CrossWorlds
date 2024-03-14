#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy_VehicleFlying_Attack.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"

#include "UIMinigame_Manager.h"
#include "Character_Biplane_Bullet.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

#include "Pool.h"
#include "Enemy_Biplane_Bullet.h"
CState_Enemy_VehicleFlying_Attack::CState_Enemy_VehicleFlying_Attack(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_Enemy_VehicleFlying_Attack::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    m_bShoot = false;
}

void CState_Enemy_VehicleFlying_Attack::Tick_State(_float fTimeDelta)
{
    CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
    if (nullptr == pPlayer)
        return;
    CCharacter* pCharacter = pPlayer->Get_Character();
    if (nullptr == pCharacter)
        return;

    Vec4 vMyPos = m_pTransformCom->Get_Position();
    Vec4 vPlayerPos = pCharacter->Get_CharacterTransformCom()->Get_Position();

    _float fDistance = (vPlayerPos - vMyPos).Length();

    if (40.f < fDistance)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
        return;
    }

    Vec3 vDestLook = XMVector3Normalize(vPlayerPos - vMyPos);
    Vec3 vMyLook = XMVector3Normalize(m_pTransformCom->Get_Look());

    m_pTransformCom->Rotation_Look(XMVector3Normalize(Vec3::Lerp(vMyLook, vDestLook, fTimeDelta)));

    if (fDistance > 30.f)
    {
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fFollowSpeed, fTimeDelta);
    }
        


    // АјАн
    if (false == m_bShoot)
    {
        m_fTimeAcc += fTimeDelta;

        if (1.f < m_fTimeAcc)
        {
            m_bShoot = true;
            m_fTimeAcc = 0.f;
        }
    }
    else
        Shoot();
}

void CState_Enemy_VehicleFlying_Attack::Exit_State()
{
    m_bShoot = false;
    m_fTimeAcc = 0.f;
}

void CState_Enemy_VehicleFlying_Attack::Shoot()
{
    CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.pOwner = dynamic_cast<CVehicle_Flying*>(m_pVehicle);

    CEnemy_Biplane_Bullet* pBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

    if (nullptr == pBullet)
    {
        pBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));
    }
    

    if (nullptr == pBullet)
        return;

    pBullet->Set_Owner(dynamic_cast<CVehicle_Flying*>(m_pVehicle));

    CTransform* pTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vScale = pTransform->Get_Scale();
    pTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pTransform->Set_Scale(vScale);

    _matrix Matrix = m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix();
    Vec4 vStartPos = Matrix.r[CTransform::STATE_POSITION];

    vStartPos += XMVector3Normalize(m_pTransformCom->Get_Look()) * 0.5f;
    pTransform->Set_State(CTransform::STATE_POSITION, vStartPos);


    CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    Vec4 vMyPos = m_pTransformCom->Get_Position();
    Vec4 vPlayerPos = pCharacter->Get_CharacterTransformCom()->Get_Position();
    pTransform->Rotation_Look(XMVector3Normalize(vPlayerPos - vMyPos));

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pBullet)))
        MSG_BOX("Generate Bullet Failed.");

    GI->Play_Sound(L"Grandprix_Bullet_Default_2.wav", CHANNELID::SOUND_CLOTH, 1.f, true);

    m_bShoot = false;
}

CState_Enemy_VehicleFlying_Attack* CState_Enemy_VehicleFlying_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Attack* pInstance = new CState_Enemy_VehicleFlying_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Attack::Free()
{
    __super::Free();
}

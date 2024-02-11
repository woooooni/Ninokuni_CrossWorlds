#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Skill_0.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Enemy_GuidedMissile.h"
#include "Pool.h"

#include "Vehicle_Flying_EnemyBiplane.h"

CState_EnemyBiplane_Skill_0::CState_EnemyBiplane_Skill_0(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{

}

HRESULT CState_EnemyBiplane_Skill_0::Initialize(const list<wstring>& AnimationList)
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

void CState_EnemyBiplane_Skill_0::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    m_iMissileCount = -20;
    m_fAccShootMissile = 0.f;
    
    // Ä«¸Þ¶ó..?
    m_pEngineerPlane->Set_Infinite(true, 999.f);
    
}

void CState_EnemyBiplane_Skill_0::Tick_State(_float fTimeDelta)
{
    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom();
    Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() - m_pTransformCom->Get_Position());
    m_pTransformCom->Rotation_Look(vDir);

    m_fAccShootMissile += fTimeDelta;
    if (m_fAccShootMissile >= m_fShootMissileTime)
    {
        m_fAccShootMissile = 0.f;
        Shoot_Missile();

        m_iMissileCount++;
        if (m_iMissileCount >= 20)
        {
            m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
            return;
        }
    }

    if ((nullptr != m_pFollowCamera->Get_LookAtObj()) && (m_pFollowCamera->Get_LookAtObj()->Get_ObjectType() != OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE))
    {
        m_pFollowCamera->Set_TargetObj(m_pTarget);
        m_pFollowCamera->Set_LookAtObj(m_pEngineerPlane);
    }
   
}

void CState_EnemyBiplane_Skill_0::Exit_State()
{
    m_iMissileCount = -20;
    m_fAccShootMissile = 0.f;
    m_pEngineerPlane->Set_Infinite(false, 0.f);
}

void CState_EnemyBiplane_Skill_0::Shoot_Missile()
{
    CEnemy_GuidedMissile* pObject = CPool<CEnemy_GuidedMissile>::Get_Obj();

    CEnemy_GuidedMissile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.bPool = true;
    ProjectileDesc.pOwner = m_pEngineerPlane;

    if (nullptr == pObject)
    {
        pObject = dynamic_cast<CEnemy_GuidedMissile*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_GuidedMissile", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));
    }

    if (nullptr == pObject)
    {
        MSG_BOX("Clone Missile Failed. : CState_EnemyBiplane_Skill_0::Shoot_Missile()");
        return;
    }

    CTransform* pMissileTransform = pObject->Get_Component_Transform();
    
    Vec3 vScale = pMissileTransform->Get_Scale();
    pMissileTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pMissileTransform->Set_Scale(vScale);

    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom();
    Vec3 vDir = pTargetTransform->Get_Position() - pMissileTransform->Get_Position();

    Vec4 vRelativePosition = Vec4(pMissileTransform->Get_Position()) + pTargetTransform->Get_RelativeOffset(Vec4(0.f, 1.f, -1.f, 1.f));
    pMissileTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vRelativePosition, 1.f));

    pMissileTransform->Rotation_Look(XMVector3Normalize(vDir));
    pMissileTransform->Rotation_Acc(XMVector3Normalize(pMissileTransform->Get_Right()), XMConvertToRadians(-90.f));
    pMissileTransform->Rotation_Acc(XMVector3Normalize(pMissileTransform->Get_Up()), m_iMissileCount * XMConvertToRadians(10.f));

    

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pObject)))
    {
        MSG_BOX("Add_GameObject Failed. : CState_EnemyBiplane_Skill_0::Shoot_Missile()");
        return;
    }
       
}


CState_EnemyBiplane_Skill_0* CState_EnemyBiplane_Skill_0::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Skill_0* pInstance = new CState_EnemyBiplane_Skill_0(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Skill_0");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Skill_0::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"
#include "Defence_Tower.h"
#include "Character_Projectile.h"
#include "Cannon_Ball.h"
#include "Particle_Manager.h"
#include "State_CannonTower_Attack.h"

CState_CannonTower_Attack::CState_CannonTower_Attack(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_CannonTower_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_CannonTower_Attack::Enter_State(void* pArg)
{
    m_bShoot = false;
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_CannonTower_Attack::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() == 5)
    {
        if (false == m_bShoot)
        {
            Fire();
            m_bShoot = true;
        }
    }
        

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CDefence_Tower::DEFENCE_TOWER_STATE::TOWER_STATE_IDLE);
        

}

void CState_CannonTower_Attack::Exit_State()
{
    m_bShoot = false;
}

void CState_CannonTower_Attack::Fire()
{
    CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.pOwner = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    CCannon_Ball* pCannonBall = dynamic_cast<CCannon_Ball*>(GI->Clone_GameObject(L"Prototype_GameObject_Cannon_Ball", LAYER_TYPE::LAYER_ETC, &ProjectileDesc));
    CTransform* pTransform = pCannonBall->Get_Component<CTransform>(L"Com_Transform");

    pTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

    Vec4 vInitPosition = m_pTransformCom->Get_Position() + m_pTransformCom->Get_Look() * 1.5f;
    vInitPosition += XMVectorSet(0.f, 5.f, 0.f, 0.f);
    pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vInitPosition, 1.f));

    CPhysX_Controller* pController = pCannonBall->Get_Component<CPhysX_Controller>(L"Com_Controller");
    pController->Set_EnterLevel_Position(pTransform->Get_Position());

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC, pCannonBall)))
    {
        MSG_BOX("Add_GameObject Failed : CState_CannonTower_Attack::Fire");
        return;
    }

    CParticle_Manager::GetInstance()->Generate_Particle(L"Particle_Defence_CannonTower_Fire_0", pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
}

CState_CannonTower_Attack* CState_CannonTower_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_CannonTower_Attack* pInstance = new CState_CannonTower_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_CannonTower_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CannonTower_Attack::Free()
{
    __super::Free();
}

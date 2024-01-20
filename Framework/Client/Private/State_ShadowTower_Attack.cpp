#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Defence_Tower.h"
#include "State_ShadowTower_Attack.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Shadow_Ball.h"

#include "Effect_Manager.h"

CState_ShadowTower_Attack::CState_ShadowTower_Attack(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_ShadowTower_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_ShadowTower_Attack::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_bShoot = false;
}

void CState_ShadowTower_Attack::Tick_State(_float fTimeDelta)
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

void CState_ShadowTower_Attack::Exit_State()
{
    m_pTower->Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
    m_bShoot = false;
}

void CState_ShadowTower_Attack::Fire()
{
    m_pTower->Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
    _matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
    WorldMatrix.r[CTransform::STATE_POSITION] += XMVectorSet(0.f, 1.f, 0.f, 0.f);
    CEffect_Manager::GetInstance()->Generate_Effect(L"Defence_ShadowTower_Fire_0", WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, 0.f), m_pOwner);

    for (_uint i = 0; i < 4; ++i)
    {
        CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

        CShadow_Ball* pShadowBall = dynamic_cast<CShadow_Ball*>(GI->Clone_GameObject(L"Prototype_GameObject_Shadow_Ball", LAYER_TYPE::LAYER_ETC, &ProjectileDesc));
        CTransform* pTransform = pShadowBall->Get_Component<CTransform>(L"Com_Transform");

        pTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

        Vec4 vInitPosition = m_pTransformCom->Get_Position();
        vInitPosition += XMVectorSet(0, 1.f, 0, 0.f);

        switch (i)
        {
        case 0:
            vInitPosition += XMVectorSet(-1.f, 0.f, -1.f, 0.f);
            break;
        case 1:
            vInitPosition += XMVectorSet(-1.f, 0.f, 1.f, 0.f);
            break;
        case 2:
            vInitPosition += XMVectorSet(1.f, 0.f, -1.f, 0.f);
            break;
        case 3:
            vInitPosition += XMVectorSet(1.f, 0.f, 1.f, 0.f);
            break;
        }

        pTransform->LookAt_ForLandObject(vInitPosition);
        pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vInitPosition, 1.f));

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC, pShadowBall)))
        {
            MSG_BOX("Add_GameObject Failed : CState_ShadowTower_Attack::Fire");
            return;
        }
    }
}

CState_ShadowTower_Attack* CState_ShadowTower_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_ShadowTower_Attack* pInstance = new CState_ShadowTower_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_ShadowTower_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_ShadowTower_Attack::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Defence_Tower.h"
#include "State_CrystalTower_Attack.h"

#include "Character_Projectile.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Crystal_Ball.h"

CState_CrystalTower_Attack::CState_CrystalTower_Attack(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_CrystalTower_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_CrystalTower_Attack::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0], MIN_TWEEN_DURATION);
    m_bShoot = false;
}

void CState_CrystalTower_Attack::Tick_State(_float fTimeDelta)
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

void CState_CrystalTower_Attack::Exit_State()
{
    m_bShoot = false;
}

void CState_CrystalTower_Attack::Fire()
{
    for (_int i = -1; i <= 1; ++i)
    {
        CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

        CCrystal_Ball* pCrystalBall = dynamic_cast<CCrystal_Ball*>(GI->Clone_GameObject(L"Prototype_GameObject_Crystal_Ball", LAYER_TYPE::LAYER_ETC, &ProjectileDesc));
        CTransform* pTransform = pCrystalBall->Get_Component<CTransform>(L"Com_Transform");

        pTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

        Vec4 vInitPosition = m_pTransformCom->Get_Position();
        vInitPosition += XMVectorSet(0.f, 5.f, 0.f, 0.f);
        vInitPosition += XMVectorSet(GI->RandomFloat(-2.f, 2.f), 0.f, GI->RandomFloat(-2.f, 2.f), 0.f);
        


        pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vInitPosition, 1.f));
        pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), i * XMConvertToRadians(90.f));

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC, pCrystalBall)))
        {
            MSG_BOX("Add_GameObject Failed : CState_CrystalTower_Attack::Fire");
            return;
        }
    }

}

CState_CrystalTower_Attack* CState_CrystalTower_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_CrystalTower_Attack* pInstance = new CState_CrystalTower_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_CrystalTower_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CrystalTower_Attack::Free()
{
    __super::Free();
}

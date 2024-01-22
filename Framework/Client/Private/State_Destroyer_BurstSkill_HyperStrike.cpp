#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_BurstSkill_HyperStrike.h"

#include "Effect_Manager.h"
#include "Character_Projectile.h"

CState_Destroyer_BurstSkill_HyperStrike::CState_Destroyer_BurstSkill_HyperStrike(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_BurstSkill_HyperStrike::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_BurstSkill_HyperStrike::Enter_State(void* pArg)
{
    m_bShoot = false;
    m_pCharacter->Look_For_Target();
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;

    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_HyperStrike"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Destroyer_BurstSkill_HyperStrike::Tick_State(_float fTimeDelta)
{

    if (false == m_bShoot)
    {
        if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() >= 0.6f)
        {
            Shoot_Hammer();
            m_bShoot = true;
        }
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_BurstSkill_HyperStrike::Exit_State()
{
    m_bShoot = false;
}
HRESULT CState_Destroyer_BurstSkill_HyperStrike::Shoot_Hammer()
{
    CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
    ProjectileDesc.pOwner = m_pCharacter;

    CGameObject* pObject = nullptr;
    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Destroyer_Burst_HyperStrikeHammer", &ProjectileDesc, &pObject)))
    {
        MSG_BOX("Add GameObject Failed. : CState_Destroyer_BurstSkill_HyperStrike::Shoot_Hammer()");
        return E_FAIL;
    }

    CTransform* pTransform = pObject->Get_Component<CTransform>(L"Com_Transform");
    if (nullptr == pTransform)
    {
        MSG_BOX("Find Transform Failed. : CState_Destroyer_BurstSkill_HyperStrike::Shoot_Hammer()");
        return E_FAIL;
    }
    _matrix InitMatirx = m_pTransformCom->Get_WorldMatrix();
    InitMatirx.r[CTransform::STATE_POSITION] += XMVector3Normalize(m_pTransformCom->Get_Look()) * 10.f;
    pTransform->Set_WorldMatrix(InitMatirx);
    m_bShoot = true;

    return S_OK;
}
CState_Destroyer_BurstSkill_HyperStrike* CState_Destroyer_BurstSkill_HyperStrike::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_BurstSkill_HyperStrike* pInstance = new CState_Destroyer_BurstSkill_HyperStrike(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_BurstSkill_HyperStrike");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_BurstSkill_HyperStrike::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Effect_Manager.h"
#include "Character_Projectile.h"
#include "State_Engineer_BurstSkill_Destruction.h"


CState_Engineer_BurstSkill_Destruction::CState_Engineer_BurstSkill_Destruction(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_BurstSkill_Destruction::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_BurstSkill_Destruction::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();

    // Effect Create
    // GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_Destruction"), m_pTransformCom->Get_WorldMatrix(), m_pCharacter);

    m_pCharacter->Appear_Weapon();
    
    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_Destruction"), pTransformCom->Get_WorldMatrix(), m_pCharacter);

    if (FAILED(Generate_Cannon()))
    {
        MSG_BOX("Generate_Cannon Failed. : CState_Engineer_BurstSkill_Destruction::Generate_Cannon()");
    }
}

void CState_Engineer_BurstSkill_Destruction::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_BurstSkill_Destruction::Exit_State()
{
    
}
HRESULT CState_Engineer_BurstSkill_Destruction::Generate_Cannon()
{

    for (_int i = -1; i <= 1; i += 2)
    {
        CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
        ProjectileDesc.pOwner = m_pCharacter;

        CGameObject* pObject = nullptr;
        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Engineer_Burst_DestructionCannon", &ProjectileDesc, &pObject)))
        {
            MSG_BOX("Add GameObject Failed. : CState_Engineer_BurstSkill_Destruction::Generate_Cannon()");
            return E_FAIL;
        }

        CTransform* pTransform = pObject->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr == pTransform)
        {
            MSG_BOX("Find Transform Failed. : CState_Engineer_BurstSkill_Destruction::Generate_Cannon()");
            return E_FAIL;
        }
        _matrix InitMatirx = m_pTransformCom->Get_WorldMatrix();
        InitMatirx.r[CTransform::STATE_POSITION] += i * 2.f * XMVector3Normalize(m_pTransformCom->Get_Right());
        pTransform->Set_WorldMatrix(InitMatirx);
    }


    return S_OK;
}
CState_Engineer_BurstSkill_Destruction* CState_Engineer_BurstSkill_Destruction::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_BurstSkill_Destruction* pInstance = new CState_Engineer_BurstSkill_Destruction(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_BurstSkill_Destruction");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_BurstSkill_Destruction::Free()
{
    __super::Free();
}

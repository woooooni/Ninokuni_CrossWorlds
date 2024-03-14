#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Skill_BurstCall.h"
#include "Effect_Manager.h"
#include "Character_Projectile.h"

CState_Engineer_Skill_BurstCall::CState_Engineer_Skill_BurstCall(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Skill_BurstCall::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    
    return S_OK;
}

void CState_Engineer_Skill_BurstCall::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pCharacter->Look_For_Target();
    m_pModelCom->Set_Animation(m_AnimIndices[0], MIN_TWEEN_DURATION);

    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Engineer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_BurstCall"), pTransformCom->Get_WorldMatrix(), m_pCharacter);

    m_bFire = false;
}

void CState_Engineer_Skill_BurstCall::Tick_State(_float fTimeDelta)
{
    if(false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() >= 0.5f)
    {
        if (false == m_bFire)
        {
            Shoot();
            m_bFire = true;
        }
    }
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);


}

void CState_Engineer_Skill_BurstCall::Exit_State()
{
    if (!CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Lock_Fov())
        CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(Cam_Fov_Follow_Default);

    m_bFire = false;
}

HRESULT CState_Engineer_Skill_BurstCall::Shoot()
{

    CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
    ProjectileDesc.pOwner = m_pCharacter;

    CGameObject* pObject = nullptr;
    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Engineer_BurstCall_Missile", &ProjectileDesc, &pObject)))
    {
        MSG_BOX("Add GameObject Failed. : CState_Engineer_Skill_BurstCall::Shoot()");
        return E_FAIL;
    }

    CTransform* pTransform = pObject->Get_Component<CTransform>(L"Com_Transform");
    if (nullptr == pTransform)
    {
        MSG_BOX("Find Transform Failed. : CState_Engineer_Skill_BurstCall::Shoot()");
        return E_FAIL;
    }

    

    if (nullptr != m_pCharacter->Get_Target())
    {
        _matrix InitMatirx = m_pTransformCom->Get_WorldMatrix();
        pTransform->Set_WorldMatrix(InitMatirx);

        CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr != pTargetTransform)
        {
            InitMatirx.r[CTransform::STATE_POSITION] = pTargetTransform->Get_Position();
            pTransform->Set_WorldMatrix(InitMatirx);
        }
    }
    else
    {
        _matrix InitMatirx = m_pTransformCom->Get_WorldMatrix();
        InitMatirx.r[CTransform::STATE_POSITION] += XMVector3Normalize(m_pTransformCom->Get_Look()) * 10.f;
        pTransform->Set_WorldMatrix(InitMatirx);
    }

    

    
    return S_OK;
}

CState_Engineer_Skill_BurstCall* CState_Engineer_Skill_BurstCall::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Skill_BurstCall* pInstance = new CState_Engineer_Skill_BurstCall(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Skill_BurstCall::Free()
{
    __super::Free();
}

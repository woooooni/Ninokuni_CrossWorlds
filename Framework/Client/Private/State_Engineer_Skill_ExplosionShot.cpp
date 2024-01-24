#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Utils.h"
#include "Character_Projectile.h"
#include "State_Engineer_Skill_ExplosionShot.h"
#include "Effect_Manager.h"

CState_Engineer_Skill_ExplosionShot::CState_Engineer_Skill_ExplosionShot(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Skill_ExplosionShot::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Skill_ExplosionShot::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Engineer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
    m_pCharacter->Look_For_Target();

    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_bShoot = false;

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_ExplosionShot"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Engineer_Skill_ExplosionShot::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && (m_pModelCom->Get_CurrAnimationFrame() >= 50.f && m_pModelCom->Get_CurrAnimationFrame() <= 55.f))
    {
        if (false == m_bShoot)
        {
            Shoot();
            m_bShoot = true;
        }

        m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 5.f - (m_pModelCom->Get_Progress()), fTimeDelta);
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_Engineer_Skill_ExplosionShot::Exit_State()
{
    if (!CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Lock_Fov())
        CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(Cam_Fov_Follow_Default);
}

void CState_Engineer_Skill_ExplosionShot::Shoot()
{
    CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
    ProjectileDesc.pOwner = m_pCharacter;

    CGameObject* pBullet = GI->Clone_GameObject(L"Prototype_GameObject_Engineer_Bullet_Bomb", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
    if (nullptr == pBullet)
        return;

    CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
    Vec3 vScale = pBulletTransform->Get_Scale();
    pBulletTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
    pBulletTransform->Set_Scale(vScale);


    Vec4 vStartPosition = pBulletTransform->Get_Position();
    vStartPosition += XMVector3Normalize(pBulletTransform->Get_Look()) * 0.5f;
    vStartPosition.y += 0.7f;
    pBulletTransform->Set_State(CTransform::STATE_POSITION, vStartPosition);

    if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pBullet)))
        MSG_BOX("Generate Bullet Failed.");
}

CState_Engineer_Skill_ExplosionShot* CState_Engineer_Skill_ExplosionShot::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Skill_ExplosionShot* pInstance = new CState_Engineer_Skill_ExplosionShot(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Skill_ExplosionShot::Free()
{
    __super::Free();
}

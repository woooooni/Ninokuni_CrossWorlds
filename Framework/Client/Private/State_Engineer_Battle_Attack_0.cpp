#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Character_Projectile.h"
#include "State_Engineer_Battle_Attack_0.h"
#include "Utils.h"

CState_Engineer_Battle_Attack_0::CState_Engineer_Battle_Attack_0(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Battle_Attack_0::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Battle_Attack_0::Enter_State(void* pArg)
{
    m_iShootCount = 3;
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();
}

void CState_Engineer_Battle_Attack_0::Tick_State(_float fTimeDelta)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 5));
    CSound_Manager::GetInstance()->Play_Sound(L"Engineer_V_Atk_Short_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
    m_pCharacter->Appear_Weapon();

    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() >= 0.5f && m_pModelCom->Get_Progress() <= 0.6f)
    {
        Shoot();
        m_pCharacter->Look_For_Target();
        m_pTransformCom->Move(XMVector3Normalize(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look())), 2.f, fTimeDelta);
    }
        

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_Engineer_Battle_Attack_0::Exit_State()
{
}

void CState_Engineer_Battle_Attack_0::Shoot()
{
    if (0 == m_iShootCount)
        return;

    m_iShootCount = 0;

    for (_int i = -1; i <= 1; ++i)
    {
        CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
        ProjectileDesc.pOwner = m_pCharacter;

        CGameObject* pBullet = GI->Clone_GameObject(L"Prototype_GameObject_Engineer_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
        if (nullptr == pBullet)
            return;

        CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
        Vec3 vScale = pBulletTransform->Get_Scale();
        pBulletTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
        pBulletTransform->Set_Scale(vScale);


        Vec4 vStartPosition = pBulletTransform->Get_Position();
        vStartPosition += XMVector3Normalize(pBulletTransform->Get_Look()) * 0.5f;
        vStartPosition.y += 1.1f;
        pBulletTransform->Set_State(CTransform::STATE_POSITION, vStartPosition);

        pBulletTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(i * 10.f));

        if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pBullet)))
            MSG_BOX("Generate Bullet Failed.");
    }

}

CState_Engineer_Battle_Attack_0* CState_Engineer_Battle_Attack_0::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Battle_Attack_0* pInstance = new CState_Engineer_Battle_Attack_0(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Battle_Attack_0::Free()
{
    __super::Free();
}

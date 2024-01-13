#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Skill_SpinningAssault.h"

#include "Effect_Manager.h"
#include "Utils.h"

#include "Camera_Manager.h"
#include "Camera.h"

#include "Timer_Manager.h"
#include "Timer.h"

CState_SwordMan_Skill_SpinningAssault::CState_SwordMan_Skill_SpinningAssault(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Skill_SpinningAssault::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Skill_SpinningAssault::Enter_State(void* pArg)
{
    wstring strNumVoice = to_wstring(CUtils::Random_Int(1, 5));
    CSound_Manager::GetInstance()->Play_Sound(L"Swordsman_V_Atk_Long_" + strNumVoice + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_SpinningAssault"), pTransformCom->Get_WorldMatrix(), m_pCharacter);


    m_MotionTrailDesc.fAlphaSpeed = 0.05f;
    m_MotionTrailDesc.fBlurPower = 0.f;

    if (ELEMENTAL_TYPE::FIRE == m_pCharacter->Get_ElementalType())
    {
        m_MotionTrailDesc.vRimColor = { 1.f, 0.2f, 0.2f, 1.f };
        m_MotionTrailDesc.vBloomPower = { 1.f, 0.2f, 0.2f };
    }
    else if (ELEMENTAL_TYPE::WATER == m_pCharacter->Get_ElementalType())
    {
        m_MotionTrailDesc.vRimColor = { 0.2f, 0.8f, 1.f, 1.f };
        m_MotionTrailDesc.vBloomPower = { 0.2f, 0.8f, 1.f };
    }
    else if (ELEMENTAL_TYPE::WOOD == m_pCharacter->Get_ElementalType())
    {
        m_MotionTrailDesc.vRimColor = { 0.2f, 1.f, 0.2f, 1.f };
        m_MotionTrailDesc.vBloomPower = { 0.2f, 1.f, 0.2f };
    }
    m_MotionTrailDesc.fMotionTrailTime = 0.1f;
    m_bTrailStart = false;
}

void CState_SwordMan_Skill_SpinningAssault::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() >= 0.25f && m_pModelCom->Get_Progress() <= 0.5f)
    {
        m_pTransformCom->Move(m_pTransformCom->Get_Look(), 20.f - (m_pModelCom->Get_Progress() * 10.f), fTimeDelta);
        if (false == m_bTrailStart)
        {
            m_bTrailStart = true;
            m_pCharacter->Generate_MotionTrail(m_MotionTrailDesc);
        }
    }
    else
    {
        m_pCharacter->Stop_MotionTrail();
    }
        


    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Skill_SpinningAssault::Exit_State()
{
    m_pCharacter->Stop_MotionTrail();
    m_bTrailStart = false;
    CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(60.f);
}

CState_SwordMan_Skill_SpinningAssault* CState_SwordMan_Skill_SpinningAssault::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Skill_SpinningAssault* pInstance = new CState_SwordMan_Skill_SpinningAssault(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Skill_SpinningAssault::Free()
{
    __super::Free();
}

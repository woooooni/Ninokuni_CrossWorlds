#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_SpecialSkill_IgnorePain.h"

#include "Effect_Manager.h"

CState_Destroyer_SpecialSkill_IgnorePain::CState_Destroyer_SpecialSkill_IgnorePain(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_SpecialSkill_IgnorePain::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_SpecialSkill_IgnorePain::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_IgnorePain"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Destroyer_SpecialSkill_IgnorePain::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_SpecialSkill_IgnorePain::Exit_State()
{
    m_pCharacter->Set_SuperArmor(10.f, true);
}

CState_Destroyer_SpecialSkill_IgnorePain* CState_Destroyer_SpecialSkill_IgnorePain::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_SpecialSkill_IgnorePain* pInstance = new CState_Destroyer_SpecialSkill_IgnorePain(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_SpecialSkill_IgnorePain::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_SpecialSkill_FrozenStorm.h"

#include "Effect_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"

CState_SwordMan_SpecialSkill_FrozenStorm::CState_SwordMan_SpecialSkill_FrozenStorm(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_SpecialSkill_FrozenStorm::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_SpecialSkill_FrozenStorm::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_FrozenStorm"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_SwordMan_SpecialSkill_FrozenStorm::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_SpecialSkill_FrozenStorm::Exit_State()
{
    CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(60.f);
}

CState_SwordMan_SpecialSkill_FrozenStorm* CState_SwordMan_SpecialSkill_FrozenStorm::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_SpecialSkill_FrozenStorm* pInstance = new CState_SwordMan_SpecialSkill_FrozenStorm(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_SpecialSkill_FrozenStorm::Free()
{
    __super::Free();
}

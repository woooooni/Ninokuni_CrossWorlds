#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_SpecialSkill_TimeLab.h"
#include "Effect_Manager.h"

CState_Engineer_SpecialSkill_TimeLab::CState_Engineer_SpecialSkill_TimeLab(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_SpecialSkill_TimeLab::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_SpecialSkill_TimeLab::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_TimeLab"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Engineer_SpecialSkill_TimeLab::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_SpecialSkill_TimeLab::Exit_State()
{
    
}

CState_Engineer_SpecialSkill_TimeLab* CState_Engineer_SpecialSkill_TimeLab::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_SpecialSkill_TimeLab* pInstance = new CState_Engineer_SpecialSkill_TimeLab(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_SpecialSkill_TimeLab::Free()
{
    __super::Free();
}

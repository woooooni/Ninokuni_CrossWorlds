#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Skill_BrutalStrike.h"

CState_Destroyer_Skill_BrutalStrike::CState_Destroyer_Skill_BrutalStrike(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Skill_BrutalStrike::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Skill_BrutalStrike::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_Skill_BrutalStrike::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween())
    {
        if((m_pModelCom->Get_Progress() >= 0.25f && m_pModelCom->Get_Progress() <= 0.35f)
            || (m_pModelCom->Get_Progress() >= 0.5f && m_pModelCom->Get_Progress() <= 0.6f))
            m_pTransformCom->Move(m_pTransformCom->Get_Look(), 2.f, fTimeDelta);
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_Skill_BrutalStrike::Exit_State()
{
    
}

CState_Destroyer_Skill_BrutalStrike* CState_Destroyer_Skill_BrutalStrike::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Skill_BrutalStrike* pInstance = new CState_Destroyer_Skill_BrutalStrike(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Skill_BrutalStrike::Free()
{
    __super::Free();
}

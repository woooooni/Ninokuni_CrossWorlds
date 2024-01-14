#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "State_SwordMan_BurstSkill_MegaSlash.h"

#include "Effect_Manager.h"

CState_SwordMan_BurstSkill_MegaSlash::CState_SwordMan_BurstSkill_MegaSlash(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_BurstSkill_MegaSlash::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_BurstSkill_MegaSlash::Enter_State(void* pArg)
{
    m_pCharacter->Set_Infinite(999.f, true);
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_MegaSlash"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_SwordMan_BurstSkill_MegaSlash::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_BurstSkill_MegaSlash::Exit_State()
{
    m_pCharacter->Set_Infinite(0.f, false);
    CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(60.f);
}
CState_SwordMan_BurstSkill_MegaSlash* CState_SwordMan_BurstSkill_MegaSlash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_BurstSkill_MegaSlash* pInstance = new CState_SwordMan_BurstSkill_MegaSlash(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_BurstSkill_MegaSlash");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_BurstSkill_MegaSlash::Free()
{
    __super::Free();
}

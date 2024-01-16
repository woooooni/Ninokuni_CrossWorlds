#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Skill_SipohoningLunge.h"

#include "Effect_Manager.h"
#include "Camera.h"
#include "Camera_Manager.h"

CState_SwordMan_Skill_SipohoningLunge::CState_SwordMan_Skill_SipohoningLunge(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Skill_SipohoningLunge::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Skill_SipohoningLunge::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pCharacter->Look_For_Target();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_SipohoningLunge"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_SwordMan_Skill_SipohoningLunge::Tick_State(_float fTimeDelta)
{

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_SwordMan_Skill_SipohoningLunge::Exit_State()
{
    CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(60.f);
}


CState_SwordMan_Skill_SipohoningLunge* CState_SwordMan_Skill_SipohoningLunge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Skill_SipohoningLunge* pInstance = new CState_SwordMan_Skill_SipohoningLunge(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Skill_SipohoningLunge::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Battle_Attack_2.h"

CState_Engineer_Battle_Attack_2::CState_Engineer_Battle_Attack_2(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Battle_Attack_2::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Battle_Attack_2::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Battle_Attack_2::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() >= 0.2f && m_pModelCom->Get_Progress() <= 0.4f)
        m_pTransformCom->Move(XMVector3Normalize(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look())), 3.f, fTimeDelta);

    Input(fTimeDelta);

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_Battle_Attack_2::Exit_State()
{
    
}

void CState_Engineer_Battle_Attack_2::Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::CTRL))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
        return;
    }

    if (KEY_TAP(KEY::LBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_3);
        return;
    }
        
}

CState_Engineer_Battle_Attack_2* CState_Engineer_Battle_Attack_2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Battle_Attack_2* pInstance = new CState_Engineer_Battle_Attack_2(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Battle_Attack_2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Battle_Attack_2::Free()
{
    __super::Free();
}

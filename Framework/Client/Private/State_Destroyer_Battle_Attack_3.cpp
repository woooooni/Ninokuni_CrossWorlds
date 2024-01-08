#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Battle_Attack_3.h"

CState_Destroyer_Battle_Attack_3::CState_Destroyer_Battle_Attack_3(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Battle_Attack_3::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Battle_Attack_3::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_Battle_Attack_3::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() >= 0.2f && m_pModelCom->Get_Progress() <= 0.3f)
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 4.f, fTimeDelta);

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);

}

void CState_Destroyer_Battle_Attack_3::Exit_State()
{
    
}


CState_Destroyer_Battle_Attack_3* CState_Destroyer_Battle_Attack_3::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Battle_Attack_3* pInstance = new CState_Destroyer_Battle_Attack_3(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_Battle_Attack_3");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Battle_Attack_3::Free()
{
    __super::Free();
}

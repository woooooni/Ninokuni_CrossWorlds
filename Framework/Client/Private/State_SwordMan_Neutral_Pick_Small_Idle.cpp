#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Small_Idle.h"

CState_SwordMan_Neutral_Pick_Small_Idle::CState_SwordMan_Neutral_Pick_Small_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Small_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;


    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);

    if (nullptr != m_pCharacter->Get_Target())
    {
        CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr != pTargetTransform)
        {
            Vec4 vHandCenterPosition = {};
            Vec4 vLeftHandPosition = (m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix()).Translation();
            Vec4 vRightHandPosition = (m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix()).Translation();

            vHandCenterPosition = (vLeftHandPosition + vRightHandPosition) / 2.f;

            pTargetTransform->Set_State(CTransform::STATE_POSITION, vHandCenterPosition);
        }
    }
    else
    {
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
    }

}

void CState_SwordMan_Neutral_Pick_Small_Idle::Exit_State()
{

}

void CState_SwordMan_Neutral_Pick_Small_Idle::Input(_float fTimeDelta)
{
    if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK);

}

CState_SwordMan_Neutral_Pick_Small_Idle* CState_SwordMan_Neutral_Pick_Small_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Small_Idle* pInstance = new CState_SwordMan_Neutral_Pick_Small_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Small_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Small_Idle::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Animals.h"
#include "State_Character_Neutral_Pick_Small_Finish.h"

CState_Character_Neutral_Pick_Small_Finish::CState_Character_Neutral_Pick_Small_Finish(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Pick_Small_Finish::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    return S_OK;
}

void CState_Character_Neutral_Pick_Small_Finish::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_bLiftAway = false;
    

    CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
    if (nullptr != pTargetTransform)
    {
        pTargetTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

        Vec4 vHandCenterPosition = {};
        Vec4 vLeftHandPosition = (m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix()).Translation();
        Vec4 vRightHandPosition = (m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix()).Translation();

        vHandCenterPosition = XMVectorSetW((vLeftHandPosition + vRightHandPosition) / 2.f, 1.f);
        vHandCenterPosition.y -= 0.1f;
        pTargetTransform->Set_State(CTransform::STATE_POSITION, vHandCenterPosition);
    }
}

void CState_Character_Neutral_Pick_Small_Finish::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() <= 0.6f)
    {
        CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr != pTargetTransform)
        {
            pTargetTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

            Vec4 vHandCenterPosition = {};
            Vec4 vLeftHandPosition = (m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix()).Translation();
            Vec4 vRightHandPosition = (m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix()).Translation();

            vHandCenterPosition = XMVectorSetW((vLeftHandPosition + vRightHandPosition) / 2.f, 1.f);
            vHandCenterPosition.y -= 0.1f;
            pTargetTransform->Set_State(CTransform::STATE_POSITION, vHandCenterPosition);
        }
    }
    else
    {
        if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() > 0.6f)
        {
            if (false == m_bLiftAway)
            {
                CAnimals* pAnimal = dynamic_cast<CAnimals*>(m_pCharacter->Get_Target());
                if (nullptr == pAnimal)
                {
                    MSG_BOX("Animal Cast Failed.");
                    return;
                }

                pAnimal->Set_Lift(false);
                m_bLiftAway = true;
            }
        }
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);

    
}

void CState_Character_Neutral_Pick_Small_Finish::Exit_State()
{
    m_bLiftAway = false;
}


CState_Character_Neutral_Pick_Small_Finish* CState_Character_Neutral_Pick_Small_Finish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Pick_Small_Finish* pInstance = new CState_Character_Neutral_Pick_Small_Finish(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Pick_Small_Finish");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Pick_Small_Finish::Free()
{
    __super::Free();
}

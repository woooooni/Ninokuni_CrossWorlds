#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Pick_Large_Finish.h"
#include "Animals.h"
#include "CurlingGame_Stone.h"

CState_Character_Neutral_Pick_Large_Finish::CState_Character_Neutral_Pick_Large_Finish(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Pick_Large_Finish::Initialize(const list<wstring>& AnimationList)
{
    
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_Character_Neutral_Pick_Large_Finish::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_bLiftAway = false;
}

void CState_Character_Neutral_Pick_Large_Finish::Tick_State(_float fTimeDelta)
{
    _float fProgress  = 0.f;

    if (m_pCharacter->Get_Target()->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
        fProgress = 0.6f;
    else if (m_pCharacter->Get_Target()->Get_ObjectType() == OBJ_TYPE::OBJ_CURLINGGAME_PROP)
        fProgress = 0.3f; // 컬링 게임 오브젝트들은 더 빨리 손에서 내려놓음

    if (m_pModelCom->Get_Progress() <= fProgress)
    {
        CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr != pTargetTransform)
        {
            _float vScale[3];

            memcpy(vScale, &pTargetTransform->Get_Scale(), sizeof(Vec3));

            for (size_t i = 0; i < CTransform::STATE::STATE_POSITION; i++)
            {
                const CTransform::STATE eState = (CTransform::STATE)i;

                const Vec3 vDir = Vec3(m_pTransformCom->Get_State(eState)).Normalized();

                pTargetTransform->Set_State(eState, vDir * vScale[i]);
            }

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
        if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() > fProgress)
        {
            if (nullptr != m_pCharacter->Get_Target())
            {
                if (false == m_bLiftAway)
                {
                    if (m_pCharacter->Get_Target()->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
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
                    else if (m_pCharacter->Get_Target()->Get_ObjectType() == OBJ_TYPE::OBJ_CURLINGGAME_PROP)
                    {
                        CCurlingGame_Stone* pStone = dynamic_cast<CCurlingGame_Stone*>(m_pCharacter->Get_Target());
                        if (nullptr == pStone)
                        {
                            MSG_BOX("CullingGameProp Cast Failed.");
                            return;
                        }

                        pStone->PutDown();
                        m_bLiftAway = true;
                    }
                }
            }
            
        }
    }



    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
}

void CState_Character_Neutral_Pick_Large_Finish::Exit_State()
{
    m_bLiftAway = false;
}


CState_Character_Neutral_Pick_Large_Finish* CState_Character_Neutral_Pick_Large_Finish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Pick_Large_Finish* pInstance = new CState_Character_Neutral_Pick_Large_Finish(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Pick_Large_Finish");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Pick_Large_Finish::Free()
{
    __super::Free();
}

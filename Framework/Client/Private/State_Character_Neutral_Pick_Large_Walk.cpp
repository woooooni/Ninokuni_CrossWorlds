#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Pick_Large_Walk.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_Character_Neutral_Pick_Large_Walk::CState_Character_Neutral_Pick_Large_Walk(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Pick_Large_Walk::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Pick_Large_Walk::Enter_State(void* pArg)
{
	m_pCharacter->Disappear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

	if (CGame_Manager::GetInstance()->Get_Kuu() != nullptr)
		CGame_Manager::GetInstance()->Get_Kuu()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CKuu::NPC_UNIQUENPC_WALK);
}

void CState_Character_Neutral_Pick_Large_Walk::Tick_State(_float fTimeDelta)
{
	if (nullptr != m_pCharacter->Get_Target())
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

			vHandCenterPosition = (vLeftHandPosition + vRightHandPosition) / 2.f;
			vHandCenterPosition.y -= 0.05f;

			pTargetTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vHandCenterPosition, 1.f));
		}
	}
	else
	{
		m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
	}
	__super::Pick_Walk_Input(fTimeDelta);
}

void CState_Character_Neutral_Pick_Large_Walk::Exit_State()
{

}


CState_Character_Neutral_Pick_Large_Walk* CState_Character_Neutral_Pick_Large_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Pick_Large_Walk* pInstance = new CState_Character_Neutral_Pick_Large_Walk(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Pick_Large_Walk");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Pick_Large_Walk::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "State_CurlingGame_Move_Character.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"


CState_CurlingGame_Move_Character::CState_CurlingGame_Move_Character(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Move_Character::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL; 
	
	return S_OK;
}

void CState_CurlingGame_Move_Character::Enter_State(void* pArg)
{
	if (m_bFirstTurn)
	{
		m_bFirstTurn = false;
		
		if (FAILED(m_pManager->Change_Turn()))
			return;
	}
	
	if (m_pManager->m_bPlayerTurn)
	{

	}
	else
	{
		if (FAILED(m_pManager->Set_AiPath()))
			return;
	}
}

void CState_CurlingGame_Move_Character::Tick_State(const _float& fTimeDelta)
{
	if (nullptr == m_pManager->m_pCurStone || nullptr == m_pManager->m_pCurParticipant)
		return;

	if (m_pManager->m_bPlayerTurn)
	{
		if (m_pManager->m_pCurStone->Is_Putted())
		{ 
			CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
			if (nullptr == pCurlingCam)
				return;

			CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
			if (nullptr != pCharacter && pCharacter->Is_Move_Input())
				pCharacter->Set_Move_Input(false);

			if (!m_bChangeCameraToStone)
			{
				pCurlingCam->Change_Target(m_pManager->m_pCurStone, 0.5f);

				m_bChangeCameraToStone = true;
			}
			else
			{
				if (!pCurlingCam->Is_ChagingTarget())
				{
					if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::DIRECTION)))
						return;
				}
			}
		}
	}
	else
	{
		Tick_NpcState(fTimeDelta);
	}
}

void CState_CurlingGame_Move_Character::LateTick_State(const _float& fTimeDelta)
{

}

void CState_CurlingGame_Move_Character::Exit_State()
{
	m_bChangeCameraToStone	= false;

	m_tNpcStateDesc.Clear();
}

HRESULT CState_CurlingGame_Move_Character::Render()
{
	return S_OK;
}

void CState_CurlingGame_Move_Character::Set_NpcStoneTransform()
{
	CModel*		pModel			= CCurlingGame_Manager::GetInstance()->m_pCurParticipant->Get_Component_Model();
	CTransform* pNpcTransform	= CCurlingGame_Manager::GetInstance()->m_pCurParticipant->Get_Component_Transform();
	CTransform* pStoneTransform = CCurlingGame_Manager::GetInstance()->m_pCurStone->Get_Component_Transform();

	if (nullptr == pModel || nullptr == pNpcTransform || nullptr == pStoneTransform)
		return;

	/* Position */
	{
		const Vec4 vLeftHandPos = (pModel->Get_SocketLocalMatrix(0) * pNpcTransform->Get_WorldMatrix()).Translation();
		const Vec4 vRightHandPos = (pModel->Get_SocketLocalMatrix(1) * pNpcTransform->Get_WorldMatrix()).Translation();

		Vec4 vStonePos = ((vLeftHandPos + vRightHandPos) * 0.5f).OneW();
		vStonePos.y -= 0.2f;

		pStoneTransform->Set_Position(vStonePos);
	}

	/* Rotation */
	{
		_float vScale[3];
		memcpy(vScale, &pStoneTransform->Get_Scale(), sizeof(Vec3));

		for (size_t i = 0; i < CTransform::STATE::STATE_POSITION; i++)
		{
			const CTransform::STATE eState = (CTransform::STATE)i;

			const Vec3 vDir = Vec3(pNpcTransform->Get_State(eState)).Normalized();

			pStoneTransform->Set_State(eState, vDir * vScale[i]);
		}
	}
}

void CState_CurlingGame_Move_Character::Tick_NpcState(const _float& fTimeDelta)
{	
	CModel* pModelCom = m_pManager->m_pCurParticipant->Get_Component_Model();
	CTransform* pTransform = m_pManager->m_pCurParticipant->Get_Component_Transform();

	if (nullptr == pModelCom || nullptr == pTransform)
		return;

	switch (m_tNpcStateDesc.eState)
	{
	case CState_CurlingGame_Move_Character::NPC_STATE::IDLE:
	{
		Set_NpcStoneTransform();

		m_tNpcStateDesc.bPutDown;

		/* Transition */
		m_tNpcStateDesc.fAcc += fTimeDelta;
		if (m_tNpcStateDesc.fIdleWaitDuration <= m_tNpcStateDesc.fAcc)
		{
			/* Transform */
			m_tNpcStateDesc.vWalkDir = 
				Vec4(m_pManager->m_tCurAiPath.vStartPoint - 
					Vec4(m_pManager->m_pCurParticipant->Get_Component_Transform()->Get_Position())).ZeroY().Normalized();

			/* Model */
			pModelCom->Set_CanChangeAnimation(true);
			{
				CAnimation* pAnim = pModelCom->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_PickWalkL");
				pAnim->Set_Loop(true);
				pModelCom->Set_Animation(pAnim->Get_AnimationName());
			}
			pModelCom->Set_CanChangeAnimation(false);

			m_tNpcStateDesc.eState = CState_CurlingGame_Move_Character::NPC_STATE::WALK;
		}
	}
		break;
	case CState_CurlingGame_Move_Character::NPC_STATE::WALK:
	{
		/* Transform */
		pTransform->Translate(m_tNpcStateDesc.vWalkDir.xyz() * m_tNpcStateDesc.fWalkSpeed * fTimeDelta);

		_float fDist = (m_pManager->m_tCurAiPath.vStartPoint - Vec4(pTransform->Get_Position())).Length();

		Set_NpcStoneTransform();

		/* Transition */
		if (fDist < m_tNpcStateDesc.fWalkDistThreshold)
		{
			/* Transform */
			{
				Vec4 vLookAt = Vec4(pTransform->Get_Position()) + (m_pManager->m_tCurAiPath.vLaunchDir * 5.f).ZeroY().ZeroW();
				pTransform->LookAt_ForLandObject(vLookAt.OneW());
			}

			/* Model */
			pModelCom->Set_CanChangeAnimation(true);
			{
				CAnimation* pAnim = pModelCom->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_PickFinishL");
				pAnim->Set_Loop(false);
				pModelCom->Set_Animation(pAnim->Get_AnimationName());
			}
			pModelCom->Set_CanChangeAnimation(false);

			m_tNpcStateDesc.eState = CState_CurlingGame_Move_Character::NPC_STATE::PUTDOWN;
		}
	}
		break;
	case CState_CurlingGame_Move_Character::NPC_STATE::PUTDOWN :
	{
		/* Transition */
		if (!pModelCom->Is_Tween() && 0.5f <= pModelCom->Get_Progress())
		{
			m_pManager->m_pCurStone->PutDown();
			m_tNpcStateDesc.eState = CState_CurlingGame_Move_Character::NPC_STATE::STONE_LERPING;
		}
		
		Set_NpcStoneTransform();
	}
		break;
	case CState_CurlingGame_Move_Character::NPC_STATE::STONE_LERPING:
	{
		if (pModelCom->Is_Finish() && L"SKM_Destroyer_Merge.ao|Destroyer_PickFinishL" == pModelCom->Get_CurrAnimation()->Get_AnimationName())
		{
			pModelCom->Set_CanChangeAnimation(true);
			pModelCom->Set_Animation(L"SKM_Destroyer_Merge.ao|Destroyer_NeutralStand");
			pModelCom->Set_CanChangeAnimation(false);
		}

		/* Transition */
		if (m_pManager->m_pCurStone->Is_Putted())
		{
			CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
			if (nullptr == pCurlingCam)
				return;

			if (!m_bChangeCameraToStone)
			{
				pCurlingCam->Change_Target(m_pManager->m_pCurStone, 0.5f);

				m_bChangeCameraToStone = true;
			}
			else
			{
				if (!pCurlingCam->Is_ChagingTarget())
				{
					if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::DIRECTION)))
						return;
				}
			}
		}
	}
		break;
	default:
		break;
	}
}

CState_CurlingGame_Move_Character* CState_CurlingGame_Move_Character::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Move_Character* pInstance = new CState_CurlingGame_Move_Character(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Move_Character");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Move_Character::Free()
{
	__super::Free();
}


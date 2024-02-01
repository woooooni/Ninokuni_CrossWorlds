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

		m_pManager->m_pCurParticipant->Get_Component_Transform()->Set_Position(m_pManager->m_tCurAiPath.vStartPoint);

		const Vec4 vLookAt = m_pManager->m_tCurAiPath.vStartPoint + (m_pManager->m_tCurAiPath.vLaunchDir * 5.f).ZeroY().ZeroW().Normalized();
		m_pManager->m_pCurParticipant->Get_Component_Transform()->LookAt_ForLandObject(vLookAt);
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
		Set_NpcStoneTransform();

		if (KEY_TAP(KEY::Q))
		{
			CCamera_CurlingGame* pCurlingCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_CurCamera());
			if (nullptr == pCurlingCam)
				return;

			if (!m_bChangeCameraToStone)
			{
				Vec4 vPos = m_pManager->m_pCurStone->Get_Transform()->Get_Position();
				vPos.y = -3.4f;
				m_pManager->m_pCurStone->Get_Transform()->Set_Position(vPos.OneW());

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
}

void CState_CurlingGame_Move_Character::LateTick_State(const _float& fTimeDelta)
{
	if (nullptr == m_pManager->m_pCurStone || nullptr == m_pManager->m_pCurParticipant)
		return;

	if (m_pManager->m_bPlayerTurn)
	{

	}
	else
	{

	}
}

void CState_CurlingGame_Move_Character::Exit_State()
{
	m_bChangeCameraToStone = false;
}

HRESULT CState_CurlingGame_Move_Character::Render()
{
	return S_OK;
}

void CState_CurlingGame_Move_Character::Set_NpcStoneTransform()
{
	CModel*		pModel			= m_pManager->m_pCurParticipant->Get_Component_Model();
	CTransform* pNpcTransform	= m_pManager->m_pCurParticipant->Get_Component_Transform();
	CTransform* pStoneTransform = m_pManager->m_pCurStone->Get_Component_Transform();

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


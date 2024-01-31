#include "stdafx.h"
#include "State_CurlingGame_Choose_Direction.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"


CState_CurlingGame_Choose_Direction::CState_CurlingGame_Choose_Direction(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Choose_Direction::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Choose_Direction::Enter_State(void* pArg)
{
	if (nullptr == m_pManager->m_pCurStone)
		return;

	/* Create Arrow */
	if (!m_bCloned)
	{
		m_bCloned = true;

		CGameObject* pClone = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP,
			TEXT("Prorotype_GameObject_CurlingGame_Arrow"), nullptr, &pClone)))
			return;

		if (nullptr != pClone)
		{
			m_pArrow = dynamic_cast<CCurlingGame_Arrow*>(pClone);
			if (nullptr == m_pArrow)
				return;

			m_pArrowTransform = dynamic_cast<CTransform*>(m_pArrow->Get_Component<CTransform>(L"Com_Transform"));
			if (nullptr == m_pArrowTransform)
				return;
		}
	}

	/* Transform Arrow */
	{
		CTransform* pOwnerTransform = nullptr;
		{
			if (m_pManager->m_bPlayerTurn)
				pOwnerTransform = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner->Get_Component_Transform();
			else
				pOwnerTransform = m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].pOwner->Get_Component_Transform();
		}
		
		/* Position */
		{
			Vec4 vPosition = m_pManager->m_tParticipants[0].pOwner->Get_Component_Transform()->Get_Position();
			vPosition.y += 2.f;
			m_pArrow->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, vPosition);
		}

		/* Look */
		{
			const Vec4 vLookAt = Vec4(pOwnerTransform->Get_Position()) + (Vec4(pOwnerTransform->Get_Look()).ZeroY().Normalized() * -10.f);

			m_pArrowTransform->LookAt_ForLandObject(vLookAt);
			
			m_pManager->m_vCurStoneLook = Vec4(m_pArrowTransform->Get_Look()).ZeroY().Normalized() * -1.f;
		}

		/* Scale */
		m_pArrow->Start_Action();
	}

	/* Active */
	m_pArrow->Set_Active(true);

}

void CState_CurlingGame_Choose_Direction::Tick_State(const _float& fTimeDelta)
{
	if (nullptr == m_pArrow || nullptr == m_pArrowTransform)
		return;

	Control_Direction(fTimeDelta);

	if (KEY_TAP(KEY::SPACE))
	{
		if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::INTENSITY)))
			return;
	}
}

void CState_CurlingGame_Choose_Direction::LateTick_State(const _float& fTimeDelta)
{
	if (nullptr == m_pArrow || nullptr == m_pArrowTransform)
		return;
}

void CState_CurlingGame_Choose_Direction::Exit_State()
{
	m_pArrow->Set_Active(false);
}

HRESULT CState_CurlingGame_Choose_Direction::Render()
{
	return S_OK;
}

void CState_CurlingGame_Choose_Direction::Control_Direction(const _float& fTimeDelta)
{
	if (m_pManager->m_bPlayerTurn)
	{
		if (KEY_HOLD(KEY::A))
		{
			Vec4 vOffset = { -0.1f, 0.f, 3.f, 1.f };

			vOffset = m_pArrowTransform->Get_RelativeOffset(vOffset);

			Vec4 vLookAt = (Vec4)m_pArrowTransform->Get_Position() + vOffset;

			m_pArrowTransform->LookAt(vLookAt.OneW());

			m_pManager->m_vCurStoneLook = Vec4(m_pArrowTransform->Get_Look()).ZeroY().Normalized() * -1.f;

		}
		else if (KEY_HOLD(KEY::D))
		{
			Vec4 vOffset = { 0.1f, 0.f, 3.f, 1.f };

			vOffset = m_pArrowTransform->Get_RelativeOffset(vOffset);

			Vec4 vLookAt = (Vec4)m_pArrowTransform->Get_Position() + vOffset;

			m_pArrowTransform->LookAt(vLookAt.OneW());

			m_pManager->m_vCurStoneLook = Vec4(m_pArrowTransform->Get_Look()).ZeroY().Normalized() * -1.f;
		}
	}
	else
	{
	}
	

}

CState_CurlingGame_Choose_Direction* CState_CurlingGame_Choose_Direction::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Choose_Direction* pInstance = new CState_CurlingGame_Choose_Direction(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Choose_Direction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Choose_Direction::Free()
{
	__super::Free();

	Safe_Release(m_pArrow);
}


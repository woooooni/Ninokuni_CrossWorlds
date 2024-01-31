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
	/* 각 캐릭터가 스톤을 들고 있는 상태에서 시작한다. */

	CGameObject* pClone = nullptr;
	CCurlingGame_Stone::STONE_INIT_DESC tStoneInitDesc;
	
	m_pManager->m_pCurStone = nullptr;

	if (m_pManager->m_bPlayerTurn) 
	{
		/* Create Stone */
		{
			tStoneInitDesc.eStoneType = CCurlingGame_Stone::STONE_TYPE::BARREL;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, 
				TEXT("Prorotype_GameObject_CurlingGame_Stone"), &tStoneInitDesc, &pClone)))
				return;

			m_pManager->m_pCurStone = dynamic_cast<CCurlingGame_Stone*>(pClone);

			if (nullptr == m_pManager->m_pCurStone)
				return;
		}

		/* Set Player State */
		{
			CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
			if (nullptr == pCharacter)
				return;

			pCharacter->Set_Target(m_pManager->m_pCurStone);
			
			pCharacter->Set_Move_Input(true);

			if(FAILED(pCharacter->Get_Component_StateMachine()->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE)))
				return;
		}
	}
	else 
	{
		tStoneInitDesc.eStoneType = CCurlingGame_Stone::STONE_TYPE::POT;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, 
			TEXT("Prorotype_GameObject_CurlingGame_Stone"), &tStoneInitDesc, &pClone)))
			return;

		m_pManager->m_pCurStone = dynamic_cast<CCurlingGame_Stone*>(pClone);

		if (nullptr == m_pManager->m_pCurStone)
			return;
	}
}

void CState_CurlingGame_Move_Character::Tick_State(const _float& fTimeDelta)
{
	if (nullptr == m_pManager->m_pCurStone)
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

	}
}

void CState_CurlingGame_Move_Character::LateTick_State(const _float& fTimeDelta)
{
	if (nullptr == m_pManager->m_pCurStone)
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

	if (m_pManager->m_bPlayerTurn)
	{

	}
	else
	{

	}
}

HRESULT CState_CurlingGame_Move_Character::Render()
{
	return S_OK;
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


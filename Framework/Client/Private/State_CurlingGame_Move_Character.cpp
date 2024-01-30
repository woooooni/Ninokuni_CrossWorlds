#include "stdafx.h"
#include "State_CurlingGame_Move_Character.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"
#include "Camera_Manager.h"
#include "CurlingGame_Manager.h"

#include "Camera_Group.h"

#include "Player.h"
#include "Character.h"
#include "Manager_StateMachine.h"

#include "CurlingGame_Stone.h"

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

			if(FAILED(pCharacter->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE)))
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
		if (KEY_TAP(KEY::F))
		{
			m_pManager->m_pCurStone->Set_Putted(true);

			if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::DIRECTION)))
				return;
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


#include "stdafx.h"
#include "GameInstance.h"
#include "Player.h"
#include "Character.h"
#include "Character_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"


CPlayer::CPlayer()
{

}

HRESULT CPlayer::Initialize(void* pArg)
{

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (nullptr != m_pCharacter)
		m_pCharacter->Tick(fTimeDelta);
}

void CPlayer::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pCharacter)
		m_pCharacter->LateTick(fTimeDelta);
}

HRESULT CPlayer::Set_Character(CHARACTER_TYPE eType, _bool  bEnterScene)
{

	if (nullptr != m_pCharacter)
	{
		if (FAILED(m_pCharacter->Exit_Character()))
			return E_FAIL;
	}
		


	m_pCharacter = CCharacter_Manager::GetInstance()->Get_Character(eType);

	if (nullptr == m_pCharacter)
		return E_FAIL;

	if (FAILED(m_pCharacter->Enter_Character()))
		return E_FAIL;

	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(m_pCharacter);
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(m_pCharacter);
	}

	if (true == bEnterScene)
	{
		CStateMachine* pMachine = m_pCharacter->Get_Component<CStateMachine>(L"Com_StateMachine");
		if (nullptr == pMachine)
			return E_FAIL;

		pMachine->Change_State(CCharacter::STATE::NEUTRAL_DOOR_ENTER);
	}

	return S_OK;
}



CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;
	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed Create : CPlayer");
		return nullptr;
	}
		
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
}




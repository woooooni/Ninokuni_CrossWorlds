#include "stdafx.h"
#include "GameInstance.h"
#include "Player.h"
#include "Character.h"
#include "Character_Manager.h"


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

HRESULT CPlayer::Set_Character(CHARACTER_TYPE eType)
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
	Safe_Release(m_pCharacter);
}




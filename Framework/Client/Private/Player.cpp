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


HRESULT CPlayer::Set_Character(CHARACTER_TYPE eType, Vec4 vEnterPosition, _bool  bEnterScene)
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
		m_pCharacter->Set_EnterLevelPosition(XMVectorSetW(vEnterPosition, 1.f));

	return S_OK;
}

HRESULT CPlayer::Tag_Character(CHARACTER_TYPE eType)
{
	if (nullptr == m_pCharacter)
		return E_FAIL;

	if (eType >= CHARACTER_TYPE::CHARACTER_END)
		return E_FAIL;

	if (m_pCharacter->Get_CharacterType() != eType)
	{
		CTransform* pPrevCharacterTransform = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");

		if (nullptr == pPrevCharacterTransform)
			return E_FAIL;


		if (FAILED(m_pCharacter->Tag_Out()))
			return E_FAIL;

		m_pCharacter = CCharacter_Manager::GetInstance()->Get_Character(eType);

		if (nullptr == m_pCharacter)
			return E_FAIL;

		if (FAILED(m_pCharacter->Tag_In(pPrevCharacterTransform->Get_Position())))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlayer::Set_Character_Initial_Position(Vec4 vEnterPosition, _bool bEnterDoor)
{
	if (nullptr == m_pCharacter)
		return E_FAIL;

	m_pCharacter->Set_InitialPosition(XMVectorSetW(vEnterPosition, 1.f));

	if (bEnterDoor)
	{
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




#include "stdafx.h"
#include "GameInstance.h"
#include "Riding_Manager.h"

#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "Character.h"

#include "Vehicle_Udadak.h"
#include "Vehicle_Flying_Biplane.h"
#include "Vehicle_Flying_EnemyBiplane.h" 
#include "Vehicle_Flying_EnemyBoto.h"
#include "Vehicle_Object_Biplane.h"

#include "Grandprix_ItemBox.h"

#include "Grandprix_Engineer.h"

IMPLEMENT_SINGLETON(CRiding_Manager)

CRiding_Manager::CRiding_Manager()
{

}

HRESULT CRiding_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CRiding_Manager::Tick(_float fTimeDelta)
{
}

void CRiding_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CRiding_Manager::Ready_Vehicle_GameObject(LEVELID eID)
{
	CVehicle::VEHICLE_DESC UdadakDesc = {};
	UdadakDesc.fSpeed = 10.f;

	CGameObject* pUdadak = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Vehicle_Udadak"), &UdadakDesc, &pUdadak)))
		return E_FAIL;
	if (nullptr == pUdadak)
		return E_FAIL;
	if (nullptr == dynamic_cast<CVehicle_Udadak*>(pUdadak))
		return E_FAIL;
	m_pUdadak = dynamic_cast<CVehicle_Udadak*>(pUdadak);
	Safe_AddRef(m_pUdadak);

	CGameObject* pBiplane = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Vehicle_Biplane"), &UdadakDesc, &pBiplane)))
		return E_FAIL;
	if (nullptr == pBiplane)
		return E_FAIL;
	if (nullptr == dynamic_cast<CVehicle_Flying_Biplane*>(pBiplane))
		return E_FAIL;
	m_pBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(pBiplane);
	Safe_AddRef(m_pBiplane);

	pBiplane = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prototype_GameObject_Vehicle_Biplane_Object"), &UdadakDesc, &pBiplane)))
		return E_FAIL;
	if (nullptr == pBiplane)
		return E_FAIL;
	if (nullptr == dynamic_cast<CVehicle_Object_Biplane*>(pBiplane))
		return E_FAIL;
	m_pObjectPlane = dynamic_cast<CVehicle_Object_Biplane*>(pBiplane);
	Safe_AddRef(m_pObjectPlane);

	return S_OK;
}

HRESULT CRiding_Manager::Ready_Vehicle_GameObjectToLayer(LEVELID eID)
{
	if (nullptr == m_pUdadak)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pUdadak)))
		return E_FAIL;
	Safe_AddRef(m_pUdadak);

	if (eID == LEVEL_EVERMORE)
	{
		if (nullptr == m_pBiplane)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, m_pBiplane)))
			return E_FAIL;
		Safe_AddRef(m_pBiplane);

		if (nullptr == m_pObjectPlane)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pObjectPlane)))
			return E_FAIL;
	}

	return S_OK;
}

_float CRiding_Manager::Get_Character_BiplaneSpeed()
{
	if (nullptr == m_pBiplane)
		return 0.f;

	return m_pBiplane->Get_Speed();
}

void CRiding_Manager::Set_Character_BiplaneSpeed(_float fSpeed)
{
	if (nullptr == m_pBiplane)
		return;

	m_pBiplane->Set_Speed(fSpeed);
}

HRESULT CRiding_Manager::Ride_ForCharacter(VEHICLE_TYPE eType, _bool bOnOff)
{
	CCharacter* pCharacter = CUI_Manager::GetInstance()->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

	switch (eType)
	{
	case UDADAK:
		if (nullptr == m_pUdadak)
			return E_FAIL;
		if (true == bOnOff)
		{
			if (true == m_bCanRide)
			{
				if (nullptr == m_pUdadak->Get_Rider())
				{
					m_pUdadak->Set_Aboard(true);
					m_pUdadak->Ride(pCharacter);
					m_bIsRiding = true;

					GI->Stop_Sound(CHANNELID::SOUND_VEHICLE);
					GI->Play_Sound(TEXT("Veh_Udadak_Hello_1.mp3"), CHANNELID::SOUND_VEHICLE,
						GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
				}
			}
		}
		else
		{
			if (pCharacter == m_pUdadak->Get_Rider())
			{
				m_pUdadak->Set_Aboard(false);
				m_bIsRiding = false;

				GI->Stop_Sound(CHANNELID::SOUND_VEHICLE);
				GI->Play_Sound(TEXT("Veh_Udadak_Bye_1.mp3"), CHANNELID::SOUND_VEHICLE,
					GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
			}
		}
		break;

	case BIPLANE:
		if (LEVELID::LEVEL_EVERMORE == GI->Get_CurrentLevel())
		{
			if (nullptr == m_pBiplane)
				return E_FAIL;

			if (true == bOnOff)
			{
				if (nullptr == m_pBiplane->Get_Rider())
				{
					m_pBiplane->Set_Aboard(true);
					m_pBiplane->Ride(pCharacter);
					m_pBiplane->Set_Players(true);
				}
			}
			else
			{
				if (pCharacter == m_pBiplane->Get_Rider())
				{
					m_pBiplane->Set_Aboard(false);
				}
			}
		}
		break;

	default:
		break;
	}

	return S_OK;
}

void CRiding_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pObjectPlane);
	Safe_Release(m_pUdadak);
	Safe_Release(m_pBiplane);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}





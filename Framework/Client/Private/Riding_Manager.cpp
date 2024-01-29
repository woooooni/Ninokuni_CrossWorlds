#include "stdafx.h"
#include "GameInstance.h"
#include "Riding_Manager.h"

#include "UI_Manager.h"
#include "Character.h"

#include "Vehicle_Udadak.h"
#include "Vehicle_Flying_Biplane.h"
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

	CGameObject* pEnemyplane = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Vehicle_Biplane"), &UdadakDesc, &pEnemyplane)))
		return E_FAIL;
	if (nullptr == pEnemyplane)
		return E_FAIL;
	if (nullptr == dynamic_cast<CVehicle_Flying_Biplane*>(pEnemyplane))
		return E_FAIL;
	m_pEnemyPlane = dynamic_cast<CVehicle_Flying_Biplane*>(pEnemyplane);
	Safe_AddRef(m_pEnemyPlane);
	//m_pEnemyPlane->Set_Aboard(true);
	//m_pEnemyPlane->Ride(nullptr);
	
	// 모든 캐릭터를 로드한 경우에만 엔지니어를 만들 수 있다.
	if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH)
	{

	}

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
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pBiplane)))
			return E_FAIL;
		Safe_AddRef(m_pBiplane);

		if (nullptr == m_pEnemyPlane)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pEnemyPlane)))
			return E_FAIL;
		Safe_AddRef(m_pEnemyPlane);
		//m_pEnemyPlane->Set_Aboard(true);
		//m_pEnemyPlane->Ride(nullptr);
	}

	return S_OK;
}

HRESULT CRiding_Manager::Ride_ForCharacter(VEHICLE_TYPE eType, _bool bOnOff)
{
	CCharacter* pCharacter = CUI_Manager::GetInstance()->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

//	CGameObject* pBiplaneTemp = nullptr;
//	CVehicle_Flying_Biplane* pBiplane = nullptr;

	switch (eType)
	{
	case UDADAK:
		if (nullptr == m_pUdadak)
			return E_FAIL;
		if (true == bOnOff)
		{
			if (nullptr == m_pUdadak->Get_Rider())
			{
				m_pUdadak->Set_Aboard(true);
				m_pUdadak->Ride(pCharacter);
			}
		}
		else
		{
			if (pCharacter == m_pUdadak->Get_Rider())
			{
				m_pUdadak->Set_Aboard(false);
			}
		}
		break;

	case BIPLANE:
		if (LEVELID::LEVEL_EVERMORE == GI->Get_CurrentLevel())
		{
//			pBiplaneTemp = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Vehicle_Biplane"));
//			if (nullptr == pBiplaneTemp)
//				return E_FAIL;
//			if (nullptr == dynamic_cast<CVehicle_Flying_Biplane*>(pBiplaneTemp))
//				return E_FAIL;
//
//			pBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(pBiplaneTemp);

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

	Safe_Release(m_pUdadak);
	Safe_Release(m_pBiplane);

	Safe_Release(m_pEngineer);
	Safe_Release(m_pEnemyPlane);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}





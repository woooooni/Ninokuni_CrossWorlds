#include "stdafx.h"
#include "GameInstance.h"
#include "Riding_Manager.h"

#include "UI_Manager.h"
#include "Character.h"

#include "Vehicle_Udadak.h"

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

	return S_OK;
}

HRESULT CRiding_Manager::Ready_Vehicle_CameObjectToLayer(LEVELID eID)
{
	if (nullptr == m_pUdadak)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pUdadak)))
		return E_FAIL;
	Safe_AddRef(m_pUdadak);

	return S_OK;
}

HRESULT CRiding_Manager::Ride(VEHICLE_TYPE eType, _bool bOnOff)
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

	default:
		break;
	}

	return S_OK;
}

void CRiding_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pUdadak);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}





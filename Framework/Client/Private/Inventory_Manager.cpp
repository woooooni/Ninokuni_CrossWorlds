#include "stdafx.h"
#include "GameInstance.h"
#include "Inventory_Manager.h"
#include "GameItem.h"

IMPLEMENT_SINGLETON(CInventory_Manager)

CInventory_Manager::CInventory_Manager()
{

}

HRESULT CInventory_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	for (auto& iter : m_Inventory)
		iter.reserve(8);

	return S_OK;
}

void CInventory_Manager::Tick(_float fTimeDelta)
{
}

void CInventory_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CInventory_Manager::Add_Item(CGameObject* pItem)
{
	//CGameItem* pItem = dynamic_cast<CGameItem*>(pItem);
	//if(pItem->Get_ItemType())
	return S_OK;
}

_bool CInventory_Manager::Is_InInventory(CGameObject* pItem)
{
	return _bool();
}

void CInventory_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}





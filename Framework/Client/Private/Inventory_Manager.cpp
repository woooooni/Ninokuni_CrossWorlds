#include "stdafx.h"
#include "GameInstance.h"
#include "Inventory_Manager.h"
#include "GameItem.h"

#include "GameItem_Consumpsion.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

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

//	for (auto& iter : m_Inventory)
//		iter.reserve(8);

	return S_OK;
}

HRESULT CInventory_Manager::Ready_Items()
{
	// 아이템의 프로토타입을 생성한다.

//	CGameItem::ITEM_DESC ItemDesc = {};
//	ItemDesc.eCode = ITEM_CODE::CONSUMPSION_ENERGY;
//	ItemDesc.eMainCategory = CGameItem::CATEGORY_POTION;
//	ItemDesc.strName = TEXT("에너지 드링크");
//	ItemDesc.strContent = TEXT("사용 시 30분 동안 온몸에 에너지가 가득 차요. \n 일반 필드에서 사냥을 통한 경험치/골드 획득량이 100& 증가해요.");
//	
//	m_Inventory[ITEM_TYPE::CONSUMPSION].push_back(CGameItem_Consumpsion::Create(&ItemDesc));

	return S_OK;
}

void CInventory_Manager::Tick(_float fTimeDelta)
{
}

void CInventory_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CInventory_Manager::Add_Item(ITEM_TYPE eType, CGameObject* pItem)
{
	//CGameItem* pItem = dynamic_cast<CGameItem*>(pItem);
	//if(pItem->Get_ItemType())
	switch (eType)
	{
	case ITEM_TYPE::ARMOR:
		break;

	case ITEM_TYPE::EQUIPMENT:
		break;

	case ITEM_TYPE::CONSUMPSION:
	//	CGameItem_Consumpsion* pETC = dynamic_cast<CGameItem_Consumpsion*>(pItem);
	//	if (nullptr == pETC)
	//		return E_FAIL;
	//
	//	auto& pItems = m_Inventory[_uint(pETC->Get_ItemType())];
	//	
	//	auto iter = find_if(pItems.begin(), pItems.end(), [&](CGameItem_Consumpsion* iETC)
	//		{
	//			return iETC->Get_ItemCode() == pETC->Get_ItemCode();
	//		});

		break;
	}

	return S_OK;
}

void CInventory_Manager::Add_Gold(_uint iGold)
{
	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return;

	pPlayer->Increase_Gold(iGold);

	CUI_Manager::GetInstance()->Show_AddItem(ITEM_TYPE::COIN, ITEM_CODE::CONSUMPSION_GOLD, iGold);
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





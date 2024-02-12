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

	for (auto& iter : m_Inventory)
		iter.reserve(6);

	ZeroMemory(m_bHave, sizeof(_bool) * (_uint)(ITEM_CODE::CODE_END));

	Ready_Items();

	return S_OK;
}

HRESULT CInventory_Manager::Ready_Items()
{
	// 아이템의 프로토타입을 생성한다.
	CGameItem::ITEM_DESC ItemDesc = {};
	ItemDesc.eCode = ITEM_CODE::CONSUMPSION_ENERGY;
	ItemDesc.eMainCategory = CGameItem::CATEGORY_POTION;
	ItemDesc.strName = TEXT("에너지 드링크");
	ItemDesc.strContent = TEXT("사용 시 30분 동안 온몸에 에너지가 가득 차요. \n 일반 필드에서 사냥을 통한 경험치/골드 획득량이 100& 증가해요.");
	
	m_Inventory[ITEM_TYPE::CONSUMPSION].push_back(CGameItem_Consumpsion::Create(&ItemDesc));

	ItemDesc.eCode = ITEM_CODE::CONSUMPSION_HP;
	ItemDesc.eMainCategory = CGameItem::CATEGORY_POTION;
	ItemDesc.strName = TEXT("HP 포션");
	ItemDesc.strContent = TEXT("사용 시 최대 HP의 50%가 즉시 회복돼요.");

	m_Inventory[ITEM_TYPE::CONSUMPSION].push_back(CGameItem_Consumpsion::Create(&ItemDesc));

	// 포션 5개를 기본으로 세팅한다.
	for (_int i = 0; i < 5; i++)
	{
		Add_Item(ITEM_TYPE::CONSUMPSION, ITEM_CODE::CONSUMPSION_HP);
	}

	return S_OK;
}

void CInventory_Manager::Tick(_float fTimeDelta)
{
}

void CInventory_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CInventory_Manager::Add_Item(ITEM_TYPE eType, ITEM_CODE eCode)
{
	switch (eType)
	{
	case ITEM_TYPE::ARMOR:
		break;

	case ITEM_TYPE::EQUIPMENT:
		break;

	case ITEM_TYPE::CONSUMPSION:
		for (auto& iter : m_Inventory[ITEM_TYPE::CONSUMPSION])
		{
			if (nullptr != iter)
			{
				if (eCode == iter->Get_ItemCode())
				{
					iter->Add_InvenCount();
				}
			}
		}

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

HRESULT CInventory_Manager::Use_Item(ITEM_CODE eCode)
{
	// 포션에 해당하는 정보만 담는다.
	for (auto& iter : m_Inventory[ITEM_TYPE::CONSUMPSION])
	{
		if (nullptr != iter)
		{
			if (eCode == iter->Get_ItemCode())
			{
				iter->Add_InvenCount(false, 1);

				CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
				CCharacter* pCharacter = pPlayer->Get_Character();
				if (nullptr == pCharacter)
					return E_FAIL;

				pCharacter->Increase_HP(100);
				return S_OK;
			}
		}
	}

	return S_OK;
}

_bool CInventory_Manager::Is_InInventory(ITEM_CODE eType)
{
	return _bool();
}

_uint CInventory_Manager::Get_InvenCount(ITEM_CODE eCode)
{
	// 포션의 개수만 파악할 수 있도록 우선 코드를 짠다.
	_uint iCount = 0;

	if (CONSUMPSION_HP == eCode)
	{
		for (auto& iter : m_Inventory[ITEM_TYPE::CONSUMPSION])
		{
			if (nullptr != iter)
			{
				if (eCode == iter->Get_ItemCode())
				{
					iCount = iter->Get_InvenCount();

					return iCount;
				}
			}
		}
	}

	return 0;
}

void CInventory_Manager::Free()
{
	__super::Free();

	for (_uint i = 0; i < ITEM_TYPE::ITEMTYPE_END; ++i)
	{
		for (auto& iter : m_Inventory[i])
		{
			Safe_Release(iter);
		}
		m_Inventory[i].clear();
	}
	

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}





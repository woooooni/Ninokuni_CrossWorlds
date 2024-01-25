#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CInventory_Manager : public CBase
{	
	DECLARE_SINGLETON(CInventory_Manager)

public:
	enum INVENTORY_TYPE { INVEN_ARMOR, INVEN_EQUIPMENT, INVEN_CONSUMPSION, INVENTORY_END };
//	enum ITEM_CODE
//	{
//		// INVEN_ARMOR
//		WEAPON_FIRST,
//		WEAPON_SECOND,
//		WEAPON_THIRD,
//		
//		// INVEN_EQUIPMENT
//		NECKLESS,
//		RING,
//		GLOVES,
//		BOOTS,
//		HELMET,
//
//		// INVEN_CONSUMPSION
//		HP_POTION,
//		ENERGY_POTION,
//
//		ITEM_END,
//
//		ARMOR_END = WEAPON_THIRD,
//		EQUIP_END = HELMET,
//		CONSUME_END = ENERGY_POTION
//	};

private:
	CInventory_Manager();
	virtual ~CInventory_Manager() = default;

public:
	_bool Is_InInventory(CGameObject* pItem);
	const vector<class CGameItem*>& Get_Inventory(INVENTORY_TYPE eType) { return m_Inventory[_uint(eType)]; }

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	HRESULT Add_Item(CGameObject* pItem);

	HRESULT Use_Item(INVENTORY_TYPE eType, _uint iNum);
	HRESULT Use_Item(ITEM_CODE eCode);

	void Erase_Item(const INVENTORY_TYPE& eType, const _uint& iIndex){
		m_Inventory[_uint(eType)].erase(m_Inventory[_uint(eType)].begin() + iIndex);
	}

private:
	vector<class CGameItem*> m_Inventory[_uint(INVENTORY_TYPE::INVENTORY_END)];

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};
END

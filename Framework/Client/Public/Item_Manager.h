#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "GameItem_Weapon.h"
#include "GameItem_Armor.h"

BEGIN(Client)

class CItem_Manager final : public CBase
{
	DECLARE_SINGLETON(CItem_Manager)
private:
	CItem_Manager();
	virtual ~CItem_Manager() = default;

public:
	HRESULT Reserve_Manager(const wstring& strPatriclePath);
	void	Tick(_float fTimeDelta);

	// void	Add_ItemDesc(CGameItem::ITEMCATEGORY_MAIN eCategory, void* pDesc);
	HRESULT	Add_ItemDesc(const wstring& strPatriclePath);
public:
	CGameItem_Weapon::ITEMDESC_WEAPON Get_WeaponItemDesc(ITEM_CODE eCode);
	CGameItem_Armor::ITEMDESC_ARMOR Get_ArmorItemDesc(ITEM_CODE eCode);

private:
	map<ITEM_CODE, CGameItem_Weapon*> m_mapWeaponItem;
	map<ITEM_CODE, CGameItem_Armor*> m_mapArmorItem;
	// 여기에 이어서 다른 카테고리도 만들기.

public:
	virtual void Free() override;
};

END


#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "GameItem_Weapon.h"

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

public:
	// void	Add_ItemDesc(CGameItem::ITEMCATEGORY_MAIN eCategory, void* pDesc);
	HRESULT	Add_ItemDesc(const wstring& strPatriclePath);


private:
	map<ITEM_CODE, CGameItem_Weapon::ITEMDESC_WEAPON> m_mapWeaponItemDesc;

public:
	virtual void Free() override;
};

END


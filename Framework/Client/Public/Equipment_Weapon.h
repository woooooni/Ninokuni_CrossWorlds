#pragma once

#include "GameItem_Equipment.h"

BEGIN(Client)

class CEquipment_Weapon final : public CGameItem_Equipment
{
public:
	enum ITEMCATEGORY_WEAPON { WEAPON_SWORD, WEAPON_GAUNTLET, WEAPON_HAMMER, WEAPON_RIFLE, WEAPON_END };

	typedef struct tagWeaponDesc : public ITEMDESC_EQUIP
	{
		ITEMCATEGORY_WEAPON eWeaponCategory = WEAPON_END;
		_float	fAtk = 0.f;
	}ITEMDESC_WEAPON;

public:
	CEquipment_Weapon();
	CEquipment_Weapon(const CEquipment_Weapon& rhs);
	virtual ~CEquipment_Weapon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	virtual ITEMDESC Get_ItemDesc() { return eWeaponDesc; }

protected:
	ITEMDESC_WEAPON eWeaponDesc = {};

public:
	//static CEquipment_Weapon* Create();
	//virtual CGameItem* Clone(void* pArg);
	virtual void Free() override;

};

END


#pragma once

#include "GameItem.h"

BEGIN(Client)

class CGameItem_Equipment abstract : public CGameItem
{
public:
	enum ITEMCATEGORY_EQUIP { EQUIP_WEAPON, EQUIP_ARMOR, EQUIP_END };

	typedef struct tagEquipDesc : public ITEMDESC
	{
		ITEMCATEGORY_EQUIP eEquipCategory = EQUIP_END;
	}ITEMDESC_EQUIP;

public:
	CGameItem_Equipment();
	CGameItem_Equipment(const CGameItem_Equipment& rhs);
	virtual ~CGameItem_Equipment() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	virtual ITEMDESC Get_ItemDesc() PURE;

public:
	virtual void Free() override;

};

END


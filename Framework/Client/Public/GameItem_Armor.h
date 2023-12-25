#pragma once

#include "GameItem.h"

BEGIN(Client)

class CGameItem_Armor final : public CGameItem
{
public:
	enum ITEMCATEGORY_ARMOR { ARMOR_HELMET = 0, ARMOR_ARMOR, ARMOR_GLOVE, ARMOR_BOOT, ARMOR_END };

	typedef struct tagArmorDesc : public CGameItem::ITEMDESC
	{
		ITEMCATEGORY_ARMOR eArmorCategory = ARMOR_END;
		_float	fDef = 0.f;
	}ITEMDESC_ARMOR;

public:
	CGameItem_Armor();
	CGameItem_Armor(const CGameItem_Armor& rhs);
	virtual ~CGameItem_Armor() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	ITEMDESC_ARMOR Get_ItemDesc() { return m_tArmorDesc; }

protected:
	ITEMDESC_ARMOR m_tArmorDesc = {};

public:
	static CGameItem_Armor* Create(void* pArg);
	virtual void Free() override;

};

END


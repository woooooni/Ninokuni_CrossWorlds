#pragma once

#include "GameItem.h"

BEGIN(Client)
class CGameItem_Consumpsion final : public CGameItem
{
public:
	enum ITEMCATEGORY_ETC { ARMOR_HELMET = 0, ARMOR_ARMOR, ARMOR_GLOVE, ARMOR_BOOT, ETC_END };

	typedef struct tagConsumpsionDesc : public CGameItem::ITEM_DESC
	{
		ITEMCATEGORY_ETC eArmorCategory = ETC_END;
		_float	fDef = 0.f;

	}ITEMDESC_ETC;

public:
	CGameItem_Consumpsion();
	CGameItem_Consumpsion(const CGameItem_Consumpsion& rhs);
	virtual ~CGameItem_Consumpsion() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	ITEMDESC_ETC Get_ItemDesc() { return m_tArmorDesc; }

protected:
	ITEMDESC_ETC m_tArmorDesc = {};

public:
	static CGameItem_Consumpsion* Create(void* pArg);
	virtual void Free() override;

};
END


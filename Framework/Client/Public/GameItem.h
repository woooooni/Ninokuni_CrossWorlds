#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CGameItem abstract : public CBase
{
public:
	enum ITEMCATEGORY_MAIN { CATEGORY_WEAPON = 0, CATEGORY_ARMOR, CATEGORY_ACC, CATEGORY_POTION, CATEGORY_ETC, CATEGORY_END };

	typedef struct tagItemDesc
	{
		ITEM_CODE eCode = CODE_END;
		wstring strName = TEXT("");
		wstring strContent = TEXT("");
		ITEMCATEGORY_MAIN eMainCategory = CATEGORY_END;

	}ITEM_DESC;

protected:
	CGameItem();
	CGameItem(const CGameItem& rhs);
	virtual ~CGameItem() = default;

public:
	const ITEM_TYPE& Get_ItemType() { return m_eItemType; }
	const _uint Get_InvenCount() { return m_iCount; }
	const ITEM_CODE& Get_ItemCode() { return m_eItemCode; }

	static wstring Get_Content(const ITEM_CODE& eCode);
	static wstring Get_Name(const ITEM_CODE& eCode);

	void Set_InInventory(_bool bIsIn) { m_bIsIn = bIsIn; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);

public:
	virtual HRESULT Use_Item() { return S_OK; }
	
protected:
	ITEM_TYPE m_eItemType = { ITEM_TYPE::ITEMTYPE_END };
	ITEM_CODE m_eItemCode = { ITEM_CODE::CODE_END };

	ITEM_DESC m_ItemDesc;

	_uint m_iCount = { 0 };
	_bool m_bIsIn = { false };

public:
	virtual void Free() override;
};

END


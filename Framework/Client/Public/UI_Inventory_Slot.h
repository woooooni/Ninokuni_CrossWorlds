#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Inventory_Slot final : public CUI
{
public:
	enum UI_INVENSLOT_TYPE
	{ INVENSLOT_NECKLACE, INVENSLOT_SWORD1, INVENSLOT_EARRING, INVENSLOT_SWORD2,
		INVENSLOT_RING, INVENSLOT_SWORD3, INVENSLOT_HELMET, INVENSLOT_ARMOR,
		INVENSLOT_GLOVES, INVENSLOT_BOOTS, INVENSLOT_LOCK_CRYSTAL1, INVENSLOT_LOCK_CRYSTAL2, INVENSLOT_END };

protected:
	CUI_Inventory_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INVENSLOT_TYPE eType);
	CUI_Inventory_Slot(const CUI_Inventory_Slot& rhs);
	virtual ~CUI_Inventory_Slot() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_INVENSLOT_TYPE m_eType = { INVENSLOT_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Inventory_Slot* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_INVENSLOT_TYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_AddItem final : public CUI
{
public:
	enum UI_QUICKSLOT_ITEM { QUICKITEM_FIRST, QUICKITEM_SECOND, QUICKITEM_THIRD, QUICKITEM_END };

protected:
	CUI_AddItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType);
	CUI_AddItem(const CUI_AddItem& rhs);
	virtual ~CUI_AddItem() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_QUICKSLOT_ITEM m_eType = { QUICKITEM_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_AddItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

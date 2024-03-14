#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_AddItem final : public CUI
{
public:
	enum UI_QUICKSLOT_ITEM { QUICKITEM_FIRST, QUICKITEM_SECOND, QUICKITEM_THIRD, QUICKITEM_END };

public:
	typedef struct tagItemDesc : public CUI::UI_INFO
	{
		ITEM_CODE eCode = CODE_END;
		_uint iCount = 0;

	}UIITEM_DESC;

protected:
	CUI_AddItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_AddItem(const CUI_AddItem& rhs);
	virtual ~CUI_AddItem() = default;

public:
	void Set_Position(_int iOrder);
	_bool Is_Disappear() { return m_bDisappear; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UIITEM_DESC m_tItemDesc;
	UI_QUICKSLOT_ITEM m_eType = { QUICKITEM_END };

	_bool m_bDisappear = { false };
	_float m_fTimeAcc = { 0.f };
	_float2 m_vDisappearPos = _float2(0.f, 0.f);
	_float2 m_vTextPos = _float2(0.f, 0.f);

	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_AddItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

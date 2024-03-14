#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Inventory_TabBtn final : public CUI
{
public:
	enum UI_INVENTABBTN
	{ INVEN_WEAPON, INVEN_ARMOR, INVEN_ACC, INVEN_CRYSTALBALL, INVEN_ETC1, INVEN_ETC2, INVENTABBTN_END };
	enum UI_INVENBTN_TYPE { INVENBTN_UNCLICKED, INVENBTN_CLICKED, INVENBTNTYPE_END };

protected:
	CUI_Inventory_TabBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INVENBTN_TYPE eButtonType, UI_INVENTABBTN eUIType);
	CUI_Inventory_TabBtn(const CUI_Inventory_TabBtn& rhs);
	virtual ~CUI_Inventory_TabBtn() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

private: 
	CTexture* m_pFXTexture = { nullptr };
	UI_INVENTABBTN m_eUIType = { UI_INVENTABBTN::INVENTABBTN_END };
	UI_INVENBTN_TYPE m_eButtonType = { UI_INVENBTN_TYPE::INVENBTNTYPE_END };

	_bool m_bClicked = { false }; // 선택되었다

	_bool m_bAlpha = { false };
	_float m_fFXAlpha = { 1.f };
	_uint m_iPass = { 0 };

	_bool m_bResize = { false };
	_float m_fTimeAcc = { 0.f };

	_float m_fOriginfCY = { 0.f };
	_float m_fMaxfCY = { 0.f };

	_float m_fOriginfY = { 0.f };
	_float m_fMaxfY = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Inventory_TabBtn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_INVENBTN_TYPE eButtonType, UI_INVENTABBTN eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

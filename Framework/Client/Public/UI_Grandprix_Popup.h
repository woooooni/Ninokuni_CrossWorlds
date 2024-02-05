#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Grandprix_Popup final : public CUI
{
public:
	enum UI_GRANDPRIX_POPUP { POPUP_BACKGROUND, POPUP_MAIN, POPUP_END };

protected:
	CUI_Grandprix_Popup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIX_POPUP eType);
	CUI_Grandprix_Popup(const CUI_Grandprix_Popup& rhs);
	virtual ~CUI_Grandprix_Popup() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_TextureIndex(_uint iIndex);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_GRANDPRIX_POPUP m_eType = { UI_GRANDPRIX_POPUP::POPUP_END };
	_uint m_iTextureIndex = { 0 };
	_float m_fTimeAcc = { 0.f };
	_bool m_bDisappear = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Grandprix_Popup* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_GRANDPRIX_POPUP eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

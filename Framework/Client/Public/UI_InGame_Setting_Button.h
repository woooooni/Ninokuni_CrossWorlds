#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_Button final : public CUI
{
public:
	enum UI_SETTINGBUTTON {
		SETBUTTON_GRAPHIC_CUSTOM, SETBUTTON_GRAPHIC_DEFAULT,
		SETBUTTON_CAMERA_SHOULDER, SETBUTTON_CAMERA_BACK,
		SETTINGBUTTON_END };

protected:
	CUI_InGame_Setting_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTINGBUTTON eType);
	CUI_InGame_Setting_Button(const CUI_InGame_Setting_Button& rhs);
	virtual ~CUI_InGame_Setting_Button() = default;

public:
	_bool Get_Click() { return m_bClicked; }
	void Set_Click(_bool bClick) {
		if (true == bClick)
			m_iPass = 7;
		else
			m_iPass = 1;

		m_bClicked = bClick;
	}

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
	CTexture* m_pFXTextureCom = { nullptr };
	UI_SETTINGBUTTON m_eType = { SETTINGBUTTON_END };
	_bool m_bClicked = { false };
	_uint m_iPass = { 1 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_InGame_Setting_Button* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SETTINGBUTTON eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

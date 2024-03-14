#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Dialog_Window final : public CUI
{
public:
	enum UI_DIALOG{ DIALOG_NORMAL, DIALOG_YELL, DIALOG_THINKING, UIDIALOG_END };

protected:
	CUI_Dialog_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dialog_Window(const CUI_Dialog_Window& rhs);
	virtual ~CUI_Dialog_Window() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Set_Name(_tchar* pszName);
	void Set_Text(_tchar* pszText);

private:
	UI_DIALOG m_eType = { DIALOG_NORMAL };
	_tchar m_szName[MAX_PATH] = TEXT("");
	_tchar m_szInfoText[MAX_PATH] = TEXT("");

	_uint m_iMaxCount = { 0 };
	_int m_iTextCount = { 0 };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Tick_Text(_float fTimeDelta);
	void Add_Text();

public:
	static CUI_Dialog_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

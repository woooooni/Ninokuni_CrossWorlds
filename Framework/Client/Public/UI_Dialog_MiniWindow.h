#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Dialog_MiniWindow final : public CUI
{
public:
	enum UI_MINI_DIALOG{ MINIDIALOG_NORMAL, MINIDIALOG_YELL, MINIDIALOG_THINKING, MINIDIALOG_END };

protected:
	CUI_Dialog_MiniWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dialog_MiniWindow(const CUI_Dialog_MiniWindow& rhs);
	virtual ~CUI_Dialog_MiniWindow() = default;

public:
	void Set_Name(const wstring& strName);
	void Set_Contents(const wstring & strText);

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
	UI_MINI_DIALOG m_eType = { MINIDIALOG_NORMAL };

	wstring m_strName;
	wstring m_strContents;
	_float2 m_vNamePosition = _float2(0.f, 0.f);
	_float2 m_vContentsPosition = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Dialog_MiniWindow* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

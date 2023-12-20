#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Dialog_Window final : public CUI
{
public:
	enum UI_DIALOG{ MINIDIALOG_NORMAL, MINIDIALOG_YELL, MINIDIALOG_THINKING, UIDIALOG_END };

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

private:
	UI_DIALOG m_eType = { MINIDIALOG_NORMAL };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Dialog_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

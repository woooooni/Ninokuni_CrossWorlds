#pragma once

#include "UI.h"

// PlayerStatus UI

BEGIN(Client)
class CUI_PlayerEXPBar final : public CUI
{
public:
	enum UI_EXPTYPE { UIEXP_BACKGROUND, UIEXP_BAR, UIEXP_END };

protected:
	CUI_PlayerEXPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EXPTYPE eType);
	CUI_PlayerEXPBar(const CUI_PlayerEXPBar& rhs);
	virtual ~CUI_PlayerEXPBar() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_EXPTYPE m_eType = { UIEXP_END };
	_uint m_iPlayerMaxEXP = { 0 }; // UIEXP_BAR만 가진다.
	_uint m_iPlayerCurEXP = { 0 }; // UIEXP_BAR만 가진다.

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_PlayerEXPBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_EXPTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

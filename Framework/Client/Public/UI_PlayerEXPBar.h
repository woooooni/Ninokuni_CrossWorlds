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
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CCharacter* m_pPlayer = { nullptr };
	UI_EXPTYPE m_eType = { UIEXP_END };
	_uint m_iPlayerMaxEXP = { 0 }; // UIEXP_BAR만 가진다.
	_uint m_iPlayerCurEXP = { 0 }; // UIEXP_BAR만 가진다.

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 100.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vHidePosition = _float2(0.f, 0.f);

	_float m_fMaxEXP = { 0.f };
	_float m_fCurEXP = { 0.f };
	_float m_fPreEXP = { 0.f };
	_float m_fTimeAcc = { 0.f };
	_uint m_iPreLevel = { 0 };
	_bool m_bLerp = { false };

	_uint m_iPass = { 1 };
	_float m_fProgress = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Movement_BasedOnHiding(_float fTimeDelta);

public:
	static CUI_PlayerEXPBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_EXPTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

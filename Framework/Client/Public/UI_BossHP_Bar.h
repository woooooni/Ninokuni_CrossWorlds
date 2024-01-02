#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BossHP_Bar final : public CUI
{
protected:
	CUI_BossHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BossHP_Bar(const CUI_BossHP_Bar& rhs);
	virtual ~CUI_BossHP_Bar() = default;

public:
	void Set_Owner(class CBoss* pBoss);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CBoss* m_pOwner = { nullptr };
	CTexture* m_pFXTextureCom = { nullptr };
	CTexture* m_pGlowTextureCom = { nullptr };
	CTexture* m_pDissolveTextureCom = { nullptr };

	_float m_fTimeAcc = { 0.f };
	_float m_fDissolveAcc = { 0.f };
	_float m_fSinValue = { 0.f };

	_bool m_bLerp = { false };
	_float m_fMaxHP = { 0.f };
	_float m_fPreHP = { 0.f };
	_float m_fCurHP = { 0.f };

	_uint m_iPass = { 17 };
	_float m_fRatio = { 0.5f };

	_uint m_iGaugeIndex = { 0 };
	_uint m_iFXTexIndex = { 0 };

	// For Text
	_float2 m_vDividerPos = _float2(g_iWinSizeX * 0.5f - 30.f, 766.f);
	_float2 m_vCurHPPos = _float2(g_iWinSizeX * 0.5f - 80.f, 768.f);
	_float2 m_vMaxHPPos = _float2(g_iWinSizeX * 0.5f, 768.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_BossHP_Bar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

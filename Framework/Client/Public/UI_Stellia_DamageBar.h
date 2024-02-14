#pragma once

#include "UI.h"

// 중앙 상단에 위치하는 가로형의 Damage Bar

BEGIN(Client)
class CUI_Stellia_DamageBar final : public CUI
{
protected:
	CUI_Stellia_DamageBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Stellia_DamageBar(const CUI_Stellia_DamageBar& rhs);
	virtual ~CUI_Stellia_DamageBar() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Add_Damage(_int iDamage) {
		m_iCurDamage += iDamage;
	};
	void Set_CurDamage(_int iCurDamage) { m_iCurDamage = iCurDamage; }
	void Set_MaxDamage(_int iMaxDamage) {
		m_iMaxDamage = iMaxDamage;
		m_iCurDamage = 0;
	}

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CTexture* m_pGaugeTextureCom = { nullptr };
	_uint m_iPass = { 24 };

	_float m_fTimeAcc = { 0.f };

	_bool m_bLerp = { false };
	_int m_iPreDamage = { 0 };
	_int m_iMaxDamage = { 0 };
	_int m_iCurDamage = { 0 };

	_float2 m_vTextScale = _float2(0.4f, 0.4f);
	_float2 m_vCurTextPos = _float2(765.f, 70.f); // y 90.f -> 80.f
	_float2 m_vSeparatorPos = _float2(g_iWinSizeX * 0.5f, 70.f);
	_float2 m_vMaxTextPos = _float2(830.f, 70.f);

	_int m_iTextureIndex = { 0 };
	_float m_fTextureAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Stellia_DamageBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

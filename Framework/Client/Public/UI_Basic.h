#pragma once

#include "UI.h"

// Render 기능만 필요한 UI class

BEGIN(Client)
class CUI_Basic final : public CUI
{
public:
	enum UI_BASIC { UILOBBY_NICKFRAME, UILOBBY_TITLE, UILOBBY_BTNTEXT,
		UIMAPNAME_EVERMORE, UIMAPNAME_KINGDOM, UIMAPNAME_ICELAND, UIMAPNAME_FOREST,
		UISTATIC_MENUDECO_L, UISTATIC_MENUDECO_R,
		UIBASIC_END };

protected:
	CUI_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType);
	CUI_Basic(const CUI_Basic& rhs);
	virtual ~CUI_Basic() = default;

public:
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
	_bool Get_Active() { return m_bActive; }
	void Set_Active(_bool bActive) { m_bActive = bActive; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_BASIC m_eType = { UI_BASIC::UIBASIC_END };
	_float m_bActive = { true };
	_float m_bFade = { false }; // Alpha값으로 FadeIn, Out을 하는 것들을 구분함.

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Tick_FadeObject(_float fTimeDelta);
	void LateTick_FadeObject(_float fTimeDelta);

public:
	static CUI_Basic* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, UI_BASIC eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

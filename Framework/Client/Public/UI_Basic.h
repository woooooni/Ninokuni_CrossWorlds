#pragma once

#include "UI.h"

// Render 기능만 필요한 UI class

BEGIN(Client)
class CUI_Basic final : public CUI
{
public:
	enum UI_BASIC { UILOBBY_NICKFRAME, UILOBBY_TITLE, UILOBBY_BTNTEXT, UILOBBY_ANNOUNCE, UILOBBY_DICE,
		UIMAPNAME_EVERMORE, UIMAPNAME_KINGDOM, UIMAPNAME_ICELAND, UIMAPNAME_FOREST,
		UIQUEST_ACCEPT, UIQUEST_FINISH,
		TARGETARROW_LEFT, TARGETARROW_RIGHT, COSTUME_INSTALL,
		WORLDMAP_ICON,
		UISTATIC_TITLELINE, UISTATIC_MENUDECO_L, UISTATIC_MENUDECO_R,
		VEHICLE_FX, UI_ENDING, WORLDMAP_BACKCOLOR,
		UIBASIC_END };

protected:
	CUI_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType);
	CUI_Basic(const CUI_Basic& rhs);
	virtual ~CUI_Basic() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_UIPass(_uint iPass) { m_iPass = iPass; }
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
	_bool Get_Active() { return m_bActive; }
	void Set_AnnouncePosition(_float2 vBtnPos);
	void Set_WorldmapIcon();
	void Update_Position(_float2 vPosition);

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
	UI_BASIC m_eType = { UI_BASIC::UIBASIC_END };

	_bool m_bFade = { false }; // Alpha값으로 FadeIn, Out을 하는 것들을 구분함.
	_uint m_iPass = { 1 };
	_bool m_bAlpha = { false }; // Alpha값을 조정하면서 깜빡이는 UI들에게 사용함.

	vector<wstring> m_RandomNickname;
	_uint m_iTextureIndex = { 0 };
	_int m_iRandomNum = { -1 };
	_uint m_iMaxNick = { 0 };

	_bool m_bEnd = { false };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Tick_FadeObject(_float fTimeDelta);
	void LateTick_FadeObject(_float fTimeDelta);
	void Ready_Nickname();
	void Set_RandomNickname();

public:
	static CUI_Basic* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, UI_BASIC eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

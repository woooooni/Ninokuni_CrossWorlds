#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_Slot final : public CUI
{
public:
	enum UI_SETTING_SECTION { SETTING_GRAPHIC, SETTING_CAMERA, SETTING_AUDIO, SETTINGSECTION_END };
	enum UI_SLOT_ORDER { SLOT_FIRST, SLOT_SECOND, SLOT_THIRD, SLOT_FOURTH, SLOT_FIFTH, SLOT_SIXTH, SLOTORDER_END };

protected:
	CUI_InGame_Setting_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_SETTING_SECTION eSection, UI_SLOT_ORDER eType);
	CUI_InGame_Setting_Slot(const CUI_InGame_Setting_Slot& rhs);
	virtual ~CUI_InGame_Setting_Slot() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	_bool Get_Clicked() { return m_bClicked; }
	void Set_Clicked(_bool bClicked) { m_bClicked = bClicked; }

	UI_SLOT_ORDER Get_SlotIndex() { return m_eType; }

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
	UI_SETTING_SECTION m_eSectionType = { SETTINGSECTION_END };
	UI_SLOT_ORDER m_eType = { SLOTORDER_END };

	// m_pTextureCom은 Graphic이 사용한다.
	CTexture* m_pTextureCamera = { nullptr };
	CTexture* m_pTextureAudio = { nullptr };

	_bool m_bArrived = { false };
	_float2 m_vArrivedPosition = _float2(0.f, 0.f);
	_float2 m_vStartPosition = _float2(0.f, 0.f);

	_float m_fSpeed = { 0.f };
	_float m_fLimit = { 0.f };

	_bool m_bClicked = { false };
	_float m_fTimeAcc = { 0.f };
	_int m_iPass = { 9 };

	_int m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_InGame_Setting_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_SETTING_SECTION eSection, UI_SLOT_ORDER eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

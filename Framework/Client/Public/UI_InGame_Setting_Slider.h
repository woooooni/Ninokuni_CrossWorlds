#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_Slider final : public CUI
{
public:
	enum UI_SETTING_SLIDERTYPE { FIRST_SLIDER, SECOND_SLIDER, THIRD_SLIDER, SLIDERTYPE_END };

protected:
	CUI_InGame_Setting_Slider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_SLIDERTYPE eType);
	CUI_InGame_Setting_Slider(const CUI_InGame_Setting_Slider& rhs);
	virtual ~CUI_InGame_Setting_Slider() = default;

public:
	_int Get_Percent() { return m_iPercent; }
	void Set_Percent(_int iPercent) { m_iPercent = iPercent; }

	virtual void Set_Active(_bool bActive);
	void Set_IsArrived(_bool bArrived) { m_bIsArrived = bArrived; }

	void Set_CameraDefaultSetting();
	void Set_AudioDefaultSetting();

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
	virtual void On_MouseDragEnter(_float fTimeDelta) override;
	virtual void On_MouseDrag(_float fTimeDelta) override;
	virtual void On_MouseDragExit(_float fTimeDelta) override;

private:
	_float m_fMinX = { 0.f };
	_float m_fMaxX = { 0.f };
	_bool m_bIsArrived = { false };
	_bool m_bDrag = { false };

	_float m_fLength = { 0.f };
	_int m_iPercent = { 0 };

	_float2 m_vTextPos = _float2(0.f, 0.f);

	UI_SETTING_SLIDERTYPE m_eType = { SLIDERTYPE_END };
	_bool m_bIsCamera = { false }; // 카메라 section slider인가

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Set_SliderRange();

public:
	static CUI_InGame_Setting_Slider* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SETTING_SLIDERTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

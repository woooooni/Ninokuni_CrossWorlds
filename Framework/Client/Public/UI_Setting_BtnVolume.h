#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Setting_BtnVolume final : public CUI
{
public:
	enum UI_SETTING_BTNTYPE { SETBTN_MINUS, SETBTN_PLUS, BTNTYPE_END };

protected:
	CUI_Setting_BtnVolume(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_BTNTYPE eType);
	CUI_Setting_BtnVolume(const CUI_Setting_BtnVolume& rhs);
	virtual ~CUI_Setting_BtnVolume() = default;

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
	UI_SETTING_BTNTYPE m_eType = { BTNTYPE_END };

	_bool m_bResize = { false };
	_float m_fTimeAcc = { 0.f };
	_float2 m_vMinSize = _float2(0.f, 0.f);
	_float2 m_vOriginSize = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Setting_BtnVolume* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SETTING_BTNTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

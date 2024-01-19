#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_RadioBtn final : public CUI
{
public:
	enum UI_RADIOGROUP
	{ GRAPHIC_NATURAL, GRAPHIC_SHADOW, GRAPHIC_OUTLINE, GRAPHIC_BLOOM, GRAPHIC_BLUR, GRAPHIC_SSAO, GRAPHIC_PBR,
		CAMERA_AXISX, CAMERA_AXISY, RADIOGROUP_END };
	enum UI_RADIOTYPE{ RADIO_ONBTN, RADIO_OFFBTN, RADIO_END };

protected:
	CUI_InGame_Setting_RadioBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADIOGROUP eGroupType, UI_RADIOTYPE eType);
	CUI_InGame_Setting_RadioBtn(const CUI_InGame_Setting_RadioBtn& rhs);
	virtual ~CUI_InGame_Setting_RadioBtn() = default;

public:
	_bool Get_Click() { return m_bClicked; }
	void Set_Click(_bool bClick) {
		if (true == bClick)
			m_iTextureIndex = 1;
		else
			m_iTextureIndex = 0;
		
		m_bClicked = bClick;
	}

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
	UI_RADIOTYPE m_eBtnType = { RADIO_END };
	UI_RADIOGROUP m_eGroupType = { RADIOGROUP_END };

	_bool m_bClicked = { false };
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_InGame_Setting_RadioBtn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_RADIOGROUP eGroupType, UI_RADIOTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

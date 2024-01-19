#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_Tab final : public CUI
{
public:
	enum UI_SETTINGTAB
	{ SETTING_GRAPHIC, SETTING_GAME, SETTING_AUDIO, SETTINGTAB_END };

protected:
	CUI_InGame_Setting_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTINGTAB eUIType);
	CUI_InGame_Setting_Tab(const CUI_InGame_Setting_Tab& rhs);
	virtual ~CUI_InGame_Setting_Tab() = default;

public:
	virtual void Set_Active(_bool bActive) override;

	void Set_Click(_bool bClick) { m_bClicked = bClick; }
	_bool Get_Clicked() { return m_bClicked; }
	
	UI_SETTINGTAB Get_CostumeType() { return m_eUIType; }

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
	void Click_TabButton(); // 나중에 합치자

private: 
	UI_SETTINGTAB m_eUIType = { UI_SETTINGTAB::SETTINGTAB_END };
	_bool m_bClicked = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_InGame_Setting_Tab* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SETTINGTAB eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

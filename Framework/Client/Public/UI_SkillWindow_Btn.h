#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillWindow_Btn final : public CUI
{
public:
	enum UI_SKILLSUBBTN { SUBMENU_CLASSIC, SUBMENU_ACTIVE, SUBMENU_PASSIVE, SKILLSUBBTN_END };
	enum UI_SKILLBTN_TYPE { SKILLBTN_UNCLICKED, SKILLBTN_CLICKED, SKILLBTNTYPE_END };

protected:
	CUI_SkillWindow_Btn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SKILLBTN_TYPE eButtonType, UI_SKILLSUBBTN eUIType);
	CUI_SkillWindow_Btn(const CUI_SkillWindow_Btn& rhs);
	virtual ~CUI_SkillWindow_Btn() = default;

public:
	virtual void Set_Active(_bool bActive) override;

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
	UI_SKILLSUBBTN m_eUIType = { UI_SKILLSUBBTN::SKILLSUBBTN_END };
	UI_SKILLBTN_TYPE m_eButtonType = { UI_SKILLBTN_TYPE::SKILLBTNTYPE_END };
	_bool m_bClicked = { false }; // 선택되었다

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_SkillWindow_Btn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SKILLBTN_TYPE eButtonType, UI_SKILLSUBBTN eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

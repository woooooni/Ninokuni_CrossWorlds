#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillWindow_SkillSlot final : public CUI
{
public:
	enum UI_SKILLMENU_SECTION { SKILL_CLASS, SKILL_BURST, SKILL_ACTIVE, SKILL_SEPARATOR, SKILLSECTION_END };
	enum UI_SKILLMENU_SLOT { SKILLSLOT_FIRST, SKILLSLOT_SECOND, SKILLSLOT_THIRD, SKILLSLOT_END };

protected:
	CUI_SkillWindow_SkillSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_SKILLMENU_SECTION eSection, UI_SKILLMENU_SLOT eType);
	CUI_SkillWindow_SkillSlot(const CUI_SkillWindow_SkillSlot& rhs);
	virtual ~CUI_SkillWindow_SkillSlot() = default;

public:
	UI_SKILLMENU_SECTION Get_SectionType() { return m_eSection; }
	void Set_SkillSlot(CHARACTER_TYPE eType);
	virtual void Set_Active(_bool bActive) override;
	void Set_Clicked(_bool bClicked) { m_bClicked = bClicked; }

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
	CTexture* m_pFXTexture = { nullptr };

	UI_SKILLMENU_SECTION m_eSection = { SKILLSECTION_END };
	UI_SKILLMENU_SLOT m_eType = { SKILLSLOT_END };

	_bool m_bArrived = { false };
	_float2 m_vArrivedPosition = _float2(0.f, 0.f);
	_float2 m_vStartPosition = _float2(0.f, 0.f);

	_float m_fSpeed = { 0.f };
	_float m_fLimit = { 0.f };

	_bool m_bClicked = { false };
	_float m_fTimeAcc = { 0.f };
	_int m_iPass = { 9 };

	_int m_iClassTexIndex = { 0 };
	_int m_iBurstTexIndex = { 0 };
	_int m_iActiveTexIndex = { 0 };

	_int m_iFXTexIndex = { 0 };

	_bool m_bAlpha = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_SkillWindow_SkillSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_SKILLMENU_SECTION eSection, UI_SKILLMENU_SLOT eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

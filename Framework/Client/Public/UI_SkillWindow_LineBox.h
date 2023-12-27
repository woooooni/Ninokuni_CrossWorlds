#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillWindow_LineBox final : public CUI
{
public:
	enum UI_SKILLWINDOW { SKWINDOW_SUBMENU, SKWINDOW_LINEBOX, SKWINDOW_ARROW, SKWINDOW_TITLE, SKWINDOW_DESC, SKILLWINDOW_END };

protected:
	CUI_SkillWindow_LineBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SKILLWINDOW eType);
	CUI_SkillWindow_LineBox(const CUI_SkillWindow_LineBox& rhs);
	virtual ~CUI_SkillWindow_LineBox() = default;

public:
	void Set_ArrowPosition(_float fPositionY);
	void Set_TitleIndex(_uint iIndex){
		if (m_eUIType != SKWINDOW_TITLE)
			return;

		m_iTitleIndex = iIndex + 1;
	}
	void Set_iDescIndex(_int iIndex) {
		if (m_eUIType != SKWINDOW_DESC)
			return;

		m_iDescIndex = iIndex;
	}

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_SKILLWINDOW m_eUIType = { SKILLWINDOW_END };
	_bool m_bAlpha = { false };

	_uint m_iTitleIndex = { 0 };
	_int m_iDescIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_SkillWindow_LineBox* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SKILLWINDOW eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

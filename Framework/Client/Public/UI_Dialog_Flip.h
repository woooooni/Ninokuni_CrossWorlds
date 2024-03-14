#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Dialog_Flip final : public CUI
{
public:
	enum UI_DIALOGFLIP { UIFLIP_ARROW, UIFLIP_BUTTON, FLIPTYPE_END };

protected:
	CUI_Dialog_Flip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DIALOGFLIP eType);
	CUI_Dialog_Flip(const CUI_Dialog_Flip& rhs);
	virtual ~CUI_Dialog_Flip() = default;

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
	CTexture* m_pFXTextureCom = { nullptr };
	UI_DIALOGFLIP m_eType = { FLIPTYPE_END };

	_int m_iPass = { 1 };

	_bool m_bArrived = { false };
	_float m_fOriginY = { 0.f };
	_float m_fTargetY = { 0.f };

	_bool m_bClicked = { false };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Dialog_Flip* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_DIALOGFLIP eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

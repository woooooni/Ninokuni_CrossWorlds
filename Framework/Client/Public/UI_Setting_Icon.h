#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Setting_Icon final : public CUI
{
public:
	enum UI_SETTING_ICONTYPE { SETICON_GAME, SETICON_GRAPHIC, SETICON_AUDIO, SETTINGICON_END };

protected:
	CUI_Setting_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_ICONTYPE eType);
	CUI_Setting_Icon(const CUI_Setting_Icon& rhs);
	virtual ~CUI_Setting_Icon() = default;

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
	UI_SETTING_ICONTYPE m_eIconType = { SETTINGICON_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Setting_Icon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_SETTING_ICONTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

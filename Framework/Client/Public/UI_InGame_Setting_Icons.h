#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_Icons final : public CUI
{
public:
	enum UI_INGAMEETC { INGAME_ARROW, CAMERA_DEFAULT, INGAMEETC_END };

protected:
	CUI_InGame_Setting_Icons(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INGAMEETC eType);
	CUI_InGame_Setting_Icons(const CUI_InGame_Setting_Icons& rhs);
	virtual ~CUI_InGame_Setting_Icons() = default;

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
	UI_INGAMEETC m_eType = { INGAMEETC_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_InGame_Setting_Icons* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_INGAMEETC eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

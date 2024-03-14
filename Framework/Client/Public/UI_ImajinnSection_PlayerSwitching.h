#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_ImajinnSection_PlayerSwitching final : public CUI
{
public:
	enum UI_PLAYERSWITCH { SLOT_SWORDMAN, SLOT_ENGINEER, SLOT_DESTROYER, SWITCH_END };

protected:
	CUI_ImajinnSection_PlayerSwitching(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PLAYERSWITCH eSlotType);
	CUI_ImajinnSection_PlayerSwitching(const CUI_ImajinnSection_PlayerSwitching& rhs);
	virtual ~CUI_ImajinnSection_PlayerSwitching() = default;

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
	UI_PLAYERSWITCH m_eSlotType = { SWITCH_END };
	_bool m_bIsUsable = { false };
	_uint m_iPass = { 1 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_ImajinnSection_PlayerSwitching* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_PLAYERSWITCH eSlotType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

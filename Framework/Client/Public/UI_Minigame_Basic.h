#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_Basic final : public CUI
{
public:
	enum UI_MINIGAMEBASIC {	TOWERDEFENCE_MENU, TOWERDEFENCE_START, TOWERDEFENCE_GOLD,
		GRANDPRIX_FRAME, GRANDPRIX_CLASSFRAME, GRANDPRIX_SPECIALFRAME, MINIGAME_END };

protected:
	CUI_Minigame_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIGAMEBASIC eType);
	CUI_Minigame_Basic(const CUI_Minigame_Basic& rhs);
	virtual ~CUI_Minigame_Basic() = default;

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
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	UI_MINIGAMEBASIC m_eType = { MINIGAME_END };
	_bool m_bUseIndex = { false };
	_uint m_iTextureIndex = { 0 };

public:
	static CUI_Minigame_Basic* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_MINIGAMEBASIC eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

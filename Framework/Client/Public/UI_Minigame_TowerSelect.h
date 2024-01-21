#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_TowerSelect final : public CUI
{
public:
	enum UI_TOWERTYPE { TOWER_CANNON, TOWER_CRYSTAL, TOWER_FLAME, TOWER_SHADOW, SELECTBTN_END };

protected:
	CUI_Minigame_TowerSelect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TOWERTYPE eType);
	CUI_Minigame_TowerSelect(const CUI_Minigame_TowerSelect& rhs);
	virtual ~CUI_Minigame_TowerSelect() = default;

public:
	_bool Get_Click() { return m_bClicked; }
	void Set_Click(_bool bClick) {
		if (true == bClick)
			m_iPass = 7;
		else
			m_iPass = 1;

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
	CTexture* m_pFXTextureCom = { nullptr };
	UI_TOWERTYPE m_eType = { SELECTBTN_END };

	_bool m_bClicked = { false };
	_uint m_iPass = { 1 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

	void Key_Input(_float fTimeDelta);

public:
	static CUI_Minigame_TowerSelect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_TOWERTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

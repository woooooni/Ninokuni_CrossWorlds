#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WeaponSection_DefaultWeapon final : public CUI
{
public:
	enum UI_DEFAULTWEAPON { DEFAULTWEAPON_SWORDMAN, DEFAULTWEAPON_DESTROYER, DEFAULTWEAPON_ENGINEER,
		DEFAULTWEAPON_WITCH, DEFAULTWEAPON_ROGUE, DEFAULTWEAPON_END };

protected:
	CUI_WeaponSection_DefaultWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_WeaponSection_DefaultWeapon(const CUI_WeaponSection_DefaultWeapon& rhs);
	virtual ~CUI_WeaponSection_DefaultWeapon() = default;

public:
	void Set_Weapon(CHARACTER_TYPE eID);

	_bool Get_Resizable() { return m_bResizable; }
	void Set_Resizable(_bool bResize) { m_bResizable = bResize; }

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
	UI_DEFAULTWEAPON m_eWeaponType = { DEFAULTWEAPON_SWORDMAN };
	_bool m_bResizable = { true }; // 사이즈 조정이 가능한가

	_float m_fTimeAcc = { 0.f };
	_bool m_bFinish = { false };
	_bool m_bResizeStart = { false };

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_WeaponSection_DefaultWeapon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

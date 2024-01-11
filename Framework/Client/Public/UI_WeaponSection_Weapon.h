#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WeaponSection_Weapon final : public CUI
{
public:
	enum UI_WEAPONSLOT_TYPE { SLOT_WEAPON, SLOT_ELEMENTAL, SLOTTYPE_END };
	enum UI_WEAPONSLOT { WEAPON_FIRST, WEAPON_SECOND, WEAPON_THIRD, WEAPON_END };

protected:
	CUI_WeaponSection_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_WEAPONSLOT eType, UI_WEAPONSLOT_TYPE eSlotType);
	CUI_WeaponSection_Weapon(const CUI_WeaponSection_Weapon& rhs);
	virtual ~CUI_WeaponSection_Weapon() = default;
	
public:
	void Set_TextureIndex(CHARACTER_TYPE eType);
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }

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
	class CCharacter* m_pCharacter = { nullptr };
	UI_WEAPONSLOT_TYPE m_eSlotType = { SLOTTYPE_END };
	UI_WEAPONSLOT m_eType = { WEAPON_END };
	_int m_iTextureIndex = { 0 };
	_int m_iElementalIndex = { 0 };

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);
	void Movement_BasedOnHiding(_float fTimeDelta);

public:
	static CUI_WeaponSection_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_WEAPONSLOT eType, UI_WEAPONSLOT_TYPE eSlotType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

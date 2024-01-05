#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WeaponSection_Slot final : public CUI
{
public:
	enum UI_WEAPONSLOT { WEAPONSLOT_FIRST, WEAPONSLOT_SECOND, WEAPONSLOT_THIRD, WEAPONSLOT_END };

protected:
	CUI_WeaponSection_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_WEAPONSLOT eSlotType);
	CUI_WeaponSection_Slot(const CUI_WeaponSection_Slot& rhs);
	virtual ~CUI_WeaponSection_Slot() = default;

public:
	virtual void Set_CharacterType(CHARACTER_TYPE eType) override;

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
	UI_WEAPONSLOT m_eSlotType = { WEAPONSLOT_END };
	_bool m_bWear = { false }; // 무기를 착용한 상태인가

	class CUI_WeaponSection_DefaultWeapon* m_pNoWeapon = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_WeaponSection_Slot* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_WEAPONSLOT eSlotType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

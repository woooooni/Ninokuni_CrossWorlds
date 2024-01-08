#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_Background final : public CUI
{
protected:
	CUI_SkillSection_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SkillSection_Background(const CUI_SkillSection_Background& rhs);
	virtual ~CUI_SkillSection_Background() = default;

public:
	virtual void Set_CharacterType(CHARACTER_TYPE eType) override;
	virtual void Set_ElementalType(ELEMENTAL_TYPE eType) override;
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Update_SelectionIcon(_uint iSlotNum);

private:
	class CUI_WeaponSection_Selected* m_pSelected = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_SkillSection_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

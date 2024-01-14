#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Quest_Reward_Item final : public CUI
{
public:
	enum UI_QUESTREWARD_ITEM { REWARD_EXP, REWARD_COIN, REWARD_HPPOTION, REWARD_ENERGY, QUESTITEM_END };

protected:
	CUI_Quest_Reward_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Quest_Reward_Item(const CUI_Quest_Reward_Item& rhs);
	virtual ~CUI_Quest_Reward_Item() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
	void Set_Type(UI_QUESTREWARD_ITEM eType) { m_eType = eType; }

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
	UI_QUESTREWARD_ITEM m_eType = { REWARD_EXP };
	_float2 m_vTextPosition = _float2(0.f, 0.f);

	_bool m_bSetInt = { false };
	_int m_iRandomInt = { 0 };
	_float m_fRandomTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Quest_Reward_Item* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

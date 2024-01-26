#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WeaponSection_Recommend final : public CUI
{
protected:
	CUI_WeaponSection_Recommend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_WeaponSection_Recommend(const CUI_WeaponSection_Recommend& rhs);
	virtual ~CUI_WeaponSection_Recommend() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Update_Position(_uint iSlotNum);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	_bool m_bUp = { false };
	_float m_fSpeed = { 30.f };

	_uint m_iCurIndex = { 0 };
	_float2 m_vPosition[3];
	_float m_fOriginY = { 0.f };
	_float m_fGoalY = { 0.f };

public:
	static CUI_WeaponSection_Recommend* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CUI_Minigame_WorldHP final : public CUI
{
protected:
	CUI_Minigame_WorldHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_WorldHP(const CUI_Minigame_WorldHP& rhs);
	virtual ~CUI_Minigame_WorldHP() = default;

public:
	void Set_VehicleInformation(class CVehicle_Flying_EnemyBiplane* pOwner);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CVehicle_Flying_EnemyBiplane* m_pTarget = { nullptr };

	CTexture* m_pFXTextureCom = { nullptr };
	_matrix m_BarWorldMatrix = XMMatrixIdentity();

	_float m_fMaxHP = { 0.f };
	_bool m_bLerp = { false };
	_float m_fPreHP = { 0.f };
	_float m_fCurHP = { 0.f };

	ELEMENTAL_TYPE m_eElementalType = { ELEMENTAL_TYPE::LIGHT };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Minigame_WorldHP* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

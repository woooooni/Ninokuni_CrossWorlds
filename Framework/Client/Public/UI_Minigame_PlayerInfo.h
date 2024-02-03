#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_PlayerInfo final : public CUI
{
protected:
	CUI_Minigame_PlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_PlayerInfo(const CUI_Minigame_PlayerInfo& rhs);
	virtual ~CUI_Minigame_PlayerInfo() = default;

public:
	void Set_Owner(class CVehicle_Flying* pOwner);
	class CVehicle_Flying* Get_Owner() { return m_pOwner; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CVehicle_Flying* m_pOwner = { nullptr };
	class CUI_Grandprix_PlaneHP* m_pHP = { nullptr };

	_float2 m_vTextPosition = _float2(145.f, 51.f);
	_float2 m_vNumPosition = _float2(200.f, 51.f);
	_float4 m_vPowerColor = { 0.525f, 0.871f, 0.749f, 1.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Minigame_PlayerInfo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

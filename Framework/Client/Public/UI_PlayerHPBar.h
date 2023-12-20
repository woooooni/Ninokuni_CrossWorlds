#pragma once

#include "UI.h"

// PlayerStatus UI

BEGIN(Client)
class CUI_PlayerHPBar final : public CUI
{
protected:
	CUI_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlayerHPBar(const CUI_PlayerHPBar& rhs);
	virtual ~CUI_PlayerHPBar() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	wstring m_strPlayerMaxHP = L"";
	wstring m_strPlayerCurHP = L"";

	_float2 m_vDefaultPosition = _float2(203.f, 31.f);
	_float2 m_vCurHPPosition = _float2(160.f, 31.f);
	_float2 m_vMaxHPPosition = _float2(216.f, 31.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_PlayerHPBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

#pragma once

#include "UI.h"

// PlayerStatus UI

BEGIN(Client)
class CUI_PlayerInfo final : public CUI
{
protected:
	CUI_PlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlayerInfo(const CUI_PlayerInfo& rhs);
	virtual ~CUI_PlayerInfo() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_float m_fAlpha = { 0.9f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_PlayerInfo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

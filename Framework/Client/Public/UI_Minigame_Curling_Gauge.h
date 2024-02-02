#pragma once

#include "UI_Minigame_Curling_Base.h"

BEGIN(Client)
class CUI_Minigame_Curling_Gauge final : public CUI_Minigame_Curling_Base
{
protected:
	CUI_Minigame_Curling_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	CUI_Minigame_Curling_Gauge(const CUI_Minigame_Curling_Gauge& rhs);
	virtual ~CUI_Minigame_Curling_Gauge() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	virtual HRESULT	Ready_Components() override;
	HRESULT			Bind_ShaderResources();

private:
	_float	m_fProgress = { 0.f };

public:
	static CUI_Minigame_Curling_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

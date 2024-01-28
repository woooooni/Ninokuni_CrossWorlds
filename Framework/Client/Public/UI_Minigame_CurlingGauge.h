#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_CurlingGauge final : public CUI
{
public:
	enum UI_CURLINGGAUGE { GAUGE_BACK, GAUGE_BAR, CURLINGGAUGE_END };

protected:
	CUI_Minigame_CurlingGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CURLINGGAUGE eType);
	CUI_Minigame_CurlingGauge(const CUI_Minigame_CurlingGauge& rhs);
	virtual ~CUI_Minigame_CurlingGauge() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_CURLINGGAUGE m_eType = { CURLINGGAUGE_END };

	_uint	m_iPass = { 0 };
	_float	m_fProgress = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;
	HRESULT			Ready_State();
	HRESULT			Bind_ShaderResources();

private:
	void Tick_GaugeBar(_float fTimeDelta);
	void LateTick_GaugeBar(_float fTimeDelta);

public:
	static CUI_Minigame_CurlingGauge* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_CURLINGGAUGE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

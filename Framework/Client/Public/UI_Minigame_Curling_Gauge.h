#pragma once

#include "UI_Minigame_Curling_Base.h"

BEGIN(Client)
class CUI_Minigame_Curling_Gauge final : public CUI_Minigame_Curling_Base
{
private:
	CUI_Minigame_Curling_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	CUI_Minigame_Curling_Gauge(const CUI_Minigame_Curling_Gauge& rhs);
	virtual ~CUI_Minigame_Curling_Gauge() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual HRESULT Send_Message() override;
	virtual HRESULT Send_Message_Float(const _float& fValue) override;

private:
	virtual HRESULT	Ready_Components() override;
	virtual HRESULT	Ready_Default() override;
	HRESULT			Bind_ShaderResources();

private:
	LERP_FLOAT_DESC m_tLerpProgress = {};
	
public:
	static CUI_Minigame_Curling_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

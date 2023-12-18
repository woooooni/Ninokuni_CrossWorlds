#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Setting_Window final : public CUI
{
protected:
	CUI_Setting_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Setting_Window(const CUI_Setting_Window& rhs);
	virtual ~CUI_Setting_Window() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	vector<_bool> m_bApproach;
	_bool m_bResize = { false };

	_float2 m_vMinSize = _float2(0.f, 0.f); // Window가 Active됐을때 처음 사이즈
	_float2 m_vOriginSIze = _float2(0.f, 0.f); // 원래 Window의 창 사이즈
	_float2 m_vMaxSize = _float2(0.f, 0.f); // 최대 사이즈

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Setting_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

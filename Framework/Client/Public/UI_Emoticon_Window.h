#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Emoticon_Window final : public CUI
{
protected:
	CUI_Emoticon_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Emoticon_Window(const CUI_Emoticon_Window& rhs);
	virtual ~CUI_Emoticon_Window() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	vector<_bool> m_Resize;

	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMaxSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);

	_float m_fValue = { 0.f };
	_float m_fLimitY = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Emoticon_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

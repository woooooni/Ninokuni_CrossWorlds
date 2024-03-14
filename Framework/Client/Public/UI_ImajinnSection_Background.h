#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_ImajinnSection_Background final : public CUI
{
protected:
	CUI_ImajinnSection_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_ImajinnSection_Background(const CUI_ImajinnSection_Background& rhs);
	virtual ~CUI_ImajinnSection_Background() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 100.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vHidePosition = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Movement_BasedOnHiding(_float fTimeDelta);

public:
	static CUI_ImajinnSection_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

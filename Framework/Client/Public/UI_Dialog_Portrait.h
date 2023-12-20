#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Dialog_Portrait final : public CUI
{
public:
	enum UI_PORTRAIT{ PORTRAIT_FRAME, PORTRAIT_CHARACTER, PORTRAIT_END };

protected:
	CUI_Dialog_Portrait(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PORTRAIT eType);
	CUI_Dialog_Portrait(const CUI_Dialog_Portrait& rhs);
	virtual ~CUI_Dialog_Portrait() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Tick_Frame(_float fTimeDelta);
	void LateTick_Frame(_float fTimeDelta);

private:
	UI_PORTRAIT m_eType = { PORTRAIT_END };

	_int m_iTextureIndex = { -1 };

	_bool m_bArrived = { false };
	_bool m_bResize = { false };
	_bool m_bAlpha = { false };

	_float m_fSpeed = { 80.f };

	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vStartPosition = _float2(0.f, 0.f);

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vStartSize = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Dialog_Portrait* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_PORTRAIT eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

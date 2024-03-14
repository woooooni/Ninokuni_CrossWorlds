#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Grandprix_RaderFrame final : public CUI
{
public:
	enum UI_RADER { RADER_FRAME, RADER_BACKGROUND, RADER_CIRCLE, RADER_END };

protected:
	CUI_Grandprix_RaderFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADER eType);
	CUI_Grandprix_RaderFrame(const CUI_Grandprix_RaderFrame& rhs);
	virtual ~CUI_Grandprix_RaderFrame() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_RADER m_eType = { RADER_END };
	_uint m_iTextureIndex = { 0 };
	_float m_fTimeAcc = { 0.f };
	_bool m_bNext = { false };
	_bool m_bReverseAlpha = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Grandprix_RaderFrame* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_RADER eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

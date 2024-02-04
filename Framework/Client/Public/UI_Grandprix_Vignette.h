#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CUI_Grandprix_Vignette final : public CUI
{
public:
	enum UI_GRANDPRIX_VIGNETTE { VIGNETTE_RUSH, VIGNETTE_DAMAGED, VIGNETTE_END };

protected:
	CUI_Grandprix_Vignette(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIX_VIGNETTE eType);
	CUI_Grandprix_Vignette(const CUI_Grandprix_Vignette& rhs);
	virtual ~CUI_Grandprix_Vignette() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	UI_GRANDPRIX_VIGNETTE m_eType = { UI_GRANDPRIX_VIGNETTE::VIGNETTE_END };

	_uint m_iTextureIndex = { 0 };
	_float m_fTimeAcc = { 0.f };

	_bool m_bSetAlpha = { false };

public:
	static CUI_Grandprix_Vignette* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_GRANDPRIX_VIGNETTE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

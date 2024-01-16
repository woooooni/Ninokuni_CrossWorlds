#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CUI_Vignette_Fire final : public CUI
{
protected:
	CUI_Vignette_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Vignette_Fire(const CUI_Vignette_Fire& rhs);
	virtual ~CUI_Vignette_Fire() = default;

public:
	virtual void Set_Active(_bool bActive) override;

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
	_bool m_bSetAlpha = { false };

public:
	static CUI_Vignette_Fire* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

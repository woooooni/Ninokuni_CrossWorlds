#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CUI_Logo_Background final : public CUI
{
protected:
	CUI_Logo_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Logo_Background(const CUI_Logo_Background& rhs);
	virtual ~CUI_Logo_Background() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_float m_fFrame = { 0.f };
	class CUI_Flare* m_pFlare = { nullptr };

private: // For Fonts
	_bool m_bTextActive = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Logo_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

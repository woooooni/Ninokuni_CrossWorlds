#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CUI_Vignette_Ice final : public CUI
{
protected:
	CUI_Vignette_Ice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Vignette_Ice(const CUI_Vignette_Ice& rhs);
	virtual ~CUI_Vignette_Ice() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Decrease_TextureIndex();

private:
	_bool m_bIsFinished = { false }; // 텍스처가 다 돌았는가
	_float m_fTimeAcc = { 0.f };
	_uint m_iMaxTextureIndex = { 0 };
	_uint m_iTextureIndex = { 0 };
	_bool m_bAlpha = { false }; // true : 알파를 낮추고 0이되면 m_bActive = false

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Vignette_Ice* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

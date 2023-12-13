#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_MapName final : public CUI
{
protected:
	CUI_MapName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MapName(const CUI_MapName& rhs);
	virtual ~CUI_MapName() = default;

public:
	_bool Get_Active() { return m_bActive; }
	void Set_Active(_bool bActive)
	{
		if (bActive)
		{
			m_bSetAlpha = false;
			m_iTextureIndex = 0;
			m_fTimeAcc = 0.f;
			m_fAlpha = 0.1f;
		}

		m_bActive = bActive;
	}

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bActive = { true };
	_bool m_bSetAlpha = { false };

	_float m_fTimeAcc = { 0.f };
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_MapName* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

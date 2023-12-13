#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Loading_Character final : public CUI
{
protected:
	CUI_Loading_Character(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading_Character(const CUI_Loading_Character& rhs);
	virtual ~CUI_Loading_Character() = default;

public:
	_bool Get_Active() { return m_bActive; }
	void Set_Active(_bool bActive) { m_bActive = bActive; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bActive = { false };
	_bool m_bReverse = { true };
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Loading_Character* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

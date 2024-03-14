#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Cursor final : public CUI
{
protected:
	CUI_Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Cursor(const CUI_Cursor& rhs);
	virtual ~CUI_Cursor() = default;

public:
	void Set_TextureIndex(_uint iIndex) { m_iTextureIndex = iIndex; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

private:
	POINT m_ptMouse = {};
	POINT m_ptOffset = {};
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Cursor* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

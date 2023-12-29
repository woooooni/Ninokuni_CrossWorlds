#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Btn_Minimap final : public CUI
{
protected:
	CUI_Btn_Minimap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Btn_Minimap(const CUI_Btn_Minimap& rhs);
	virtual ~CUI_Btn_Minimap() = default;

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
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Btn_Minimap* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

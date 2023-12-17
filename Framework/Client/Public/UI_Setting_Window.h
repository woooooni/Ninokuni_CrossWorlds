#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Setting_Window final : public CUI
{
protected:
	CUI_Setting_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Setting_Window(const CUI_Setting_Window& rhs);
	virtual ~CUI_Setting_Window() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Setting_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

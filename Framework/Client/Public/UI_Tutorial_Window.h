#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Tutorial_Window final : public CUI
{
protected:
	CUI_Tutorial_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Tutorial_Window(const CUI_Tutorial_Window& rhs);
	virtual ~CUI_Tutorial_Window() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

private:
	_bool m_bFadeOut = { false };

public:
	static CUI_Tutorial_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
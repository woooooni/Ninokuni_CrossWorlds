#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_ImajinnSection_Background final : public CUI
{
protected:
	CUI_ImajinnSection_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_ImajinnSection_Background(const CUI_ImajinnSection_Background& rhs);
	virtual ~CUI_ImajinnSection_Background() = default;

public:
	virtual void Set_Active(_bool bActive) override;

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
	static CUI_ImajinnSection_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_MenuSeparator final : public CUI
{
protected:
	CUI_MenuSeparator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MenuSeparator(const CUI_MenuSeparator& rhs);
	virtual ~CUI_MenuSeparator() = default;

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
	static CUI_MenuSeparator* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

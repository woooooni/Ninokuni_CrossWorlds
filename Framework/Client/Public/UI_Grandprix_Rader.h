#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Grandprix_Rader final : public CUI
{
protected:
	CUI_Grandprix_Rader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Grandprix_Rader(const CUI_Grandprix_Rader& rhs);
	virtual ~CUI_Grandprix_Rader() = default;

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
	static CUI_Grandprix_Rader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

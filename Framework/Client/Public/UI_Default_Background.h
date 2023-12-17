#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Default_Background final : public CUI
{
protected:
	CUI_Default_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Default_Background(const CUI_Default_Background& rhs);
	virtual ~CUI_Default_Background() = default;

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

public:
	static CUI_Default_Background* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
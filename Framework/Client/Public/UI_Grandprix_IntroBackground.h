#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Grandprix_IntroBackground final : public CUI
{
protected:
	CUI_Grandprix_IntroBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Grandprix_IntroBackground(const CUI_Grandprix_IntroBackground& rhs);
	virtual ~CUI_Grandprix_IntroBackground() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

public:
	static CUI_Grandprix_IntroBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BtnShowSetting final : public CUI
{
protected:
	CUI_BtnShowSetting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BtnShowSetting(const CUI_BtnShowSetting& rhs);
	virtual ~CUI_BtnShowSetting() = default;

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
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_BtnShowSetting* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

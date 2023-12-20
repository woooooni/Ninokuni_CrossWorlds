#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Flare final : public CUI
{
protected:
	CUI_Flare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Flare(const CUI_Flare& rhs);
	virtual ~CUI_Flare() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bReverse = { true };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Flare* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

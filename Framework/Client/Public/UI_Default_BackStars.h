#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Default_BackStars final : public CUI
{
protected:
	CUI_Default_BackStars(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Default_BackStars(const CUI_Default_BackStars& rhs);
	virtual ~CUI_Default_BackStars() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	
private:
	_bool m_bReverse = { true };
	_uint m_iTextureIndex = { 0 };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

public:
	static CUI_Default_BackStars* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
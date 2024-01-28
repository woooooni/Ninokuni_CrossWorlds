#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_GaugeBar final : public CUI
{
protected:
	CUI_Minigame_GaugeBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_GaugeBar(const CUI_Minigame_GaugeBar& rhs);
	virtual ~CUI_Minigame_GaugeBar() = default;

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

private:
	_uint m_iValue = { 0 };
	_uint m_iTextureIndex = { 0 };

public:
	static CUI_Minigame_GaugeBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

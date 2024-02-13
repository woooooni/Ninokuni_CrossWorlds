#pragma once

#include "UI.h"

// 우측 하단에 위치하는 원형의 Gauge Bar(위치 및 텍스처 변동 가능성 있음)

BEGIN(Client)
class CUI_Stellia_GaugeBar final : public CUI
{
protected:
	CUI_Stellia_GaugeBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Stellia_GaugeBar(const CUI_Stellia_GaugeBar& rhs);
	virtual ~CUI_Stellia_GaugeBar() = default;

public:
	void Set_CurGauge(_int iGauge) { m_iCurGauge = iGauge; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CTexture* m_pGaugeTextureCom = { nullptr };
	_uint m_iPass = { 25 };

	_float m_fTimeAcc = { 0.f };

	_int m_iMaxGauge = { 0 };
	_int m_iCurGauge = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Stellia_GaugeBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

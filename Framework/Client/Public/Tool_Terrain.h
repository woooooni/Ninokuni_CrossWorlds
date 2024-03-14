#pragma once
#include "Tool.h"


BEGIN(Client)

class CTool_Terrain final : public CTool
{
private:
	CTool_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Terrain() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	_float4 m_vEye = _float4(30.f, 100.f, 0.f, 1.f);
	_float4 m_vAt  = _float4(0.f, 0.f, 0.f, 1.f);
	_float4 m_vUp  = _float4(0.f, 1.f, 0.f, 0.f);

public:
	static CTool_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
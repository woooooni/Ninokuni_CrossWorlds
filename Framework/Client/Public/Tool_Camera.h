#pragma once
#include "Tool.h"

BEGIN(Client)
class CTool_Camera final : public CTool
{
private:
	CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Camera() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:

public:
	static CTool_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
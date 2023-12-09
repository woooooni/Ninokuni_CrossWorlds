#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Particle final : public CTool
{
private:
	CTool_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Particle() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;


public:
	static CTool_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
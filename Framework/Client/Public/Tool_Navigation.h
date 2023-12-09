#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Navigation final : public CTool
{
private:
	CTool_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Navigation() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;


public:
	static CTool_Navigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
#pragma once
#include "Tool.h"

BEGIN(Client)
class CTool_Effect final : public CTool
{
private:
	CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Effect() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;


public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
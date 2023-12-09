#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Map final : public CTool
{
private:
	CTool_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Map() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;


public:
	static CTool_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END

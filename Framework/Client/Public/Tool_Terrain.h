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


public:
	static CTool_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
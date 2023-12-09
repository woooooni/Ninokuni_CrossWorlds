#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Model final : public CTool
{
private:
	CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;


public:
	static CTool_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
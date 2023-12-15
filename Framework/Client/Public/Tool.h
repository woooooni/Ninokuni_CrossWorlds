#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CTool abstract : public CBase
{
protected:
	CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta) PURE;

	virtual HRESULT Render() { return S_OK; }

protected:
	ID3D11Device* m_pDevice = nullptr; 
	ID3D11DeviceContext* m_pContext = nullptr;


public:
	virtual void Free() override;

};

END

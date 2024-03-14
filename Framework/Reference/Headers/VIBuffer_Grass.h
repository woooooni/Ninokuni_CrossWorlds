#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Grass final : public CVIBuffer
{
private:
	explicit CVIBuffer_Grass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_Grass(const CVIBuffer_Grass& rhs);
	virtual ~CVIBuffer_Grass() = default;

public:
	virtual HRESULT Initialize_Prototype() ;
	virtual HRESULT Initialize(void* pArg);
	void Tick(_float fTImeDelta);
	virtual HRESULT Render() override;

private:

public:
	static CVIBuffer_Grass* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END

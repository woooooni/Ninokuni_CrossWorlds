#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Glass final : public CVIBuffer
{
private:
	explicit CVIBuffer_Glass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_Glass(const CVIBuffer_Glass& rhs);
	virtual ~CVIBuffer_Glass() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void Tick(_float fTImeDelta);
	virtual HRESULT Render(_uint iCount);

private:
	VTXINSTANCE* m_pVertices = nullptr;
	ID3D11Buffer* m_pVBInstance = nullptr;

private:
	_uint m_iNumInstance = 0;
	_uint m_iStrideInstance = 0;

public:
	static CVIBuffer_Glass* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END

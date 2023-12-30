#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_SkyPlane final : public CVIBuffer
{
private:
	CVIBuffer_SkyPlane(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer_SkyPlane(const CVIBuffer_SkyPlane& rhs);
	virtual ~CVIBuffer_SkyPlane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_SkyPlane();



private:
	SkyPlaneType* m_pSkyPlane = nullptr;
	_uint m_iSkyPlaneResolution = 20;

public:
	static CVIBuffer_SkyPlane* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END
#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_LensFlare final : public CVIBuffer
{
private:
	explicit CVIBuffer_LensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_LensFlare(const CVIBuffer_LensFlare& rhs);
	virtual ~CVIBuffer_LensFlare() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual HRESULT Render(uint32 vertexCount, uint32 instance_count = 1, uint32 start_vertex_location = 0, uint32 start_instance_location = 0);

	void Tick(const Vec3& sunWorldPos, _float fTimeDelta);

public:
	static CVIBuffer_LensFlare* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END

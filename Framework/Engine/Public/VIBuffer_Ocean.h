#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Ocean final : public CVIBuffer
{
private:
	explicit CVIBuffer_Ocean(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_Ocean(const CVIBuffer_Ocean& rhs);
	virtual ~CVIBuffer_Ocean() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint width, _uint height, _float thick);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Ocean* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		_uint width, _uint hegiht, _float thick);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END


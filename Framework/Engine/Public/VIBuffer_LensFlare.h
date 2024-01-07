#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_LensFlare final : public CVIBuffer
{
private:
	struct CB_LENSFLARE_VS
	{
		Vec4 Position;
		Vec4 ScaleRotate;
		Vec4 Color;
	};
private:
	explicit CVIBuffer_LensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_LensFlare(const CVIBuffer_LensFlare& rhs);
	virtual ~CVIBuffer_LensFlare() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT	Initialize();

	void Tick(const Vec3& sunWorldPos, _float fTimeDelta);
	HRESULT LensFlareRender();
	void BeginSunVisibility();
	void EndSunVisiBility();

private:
	ID3D11Predicate* m_pPredicate = nullptr;
	ID3D11Query* m_pOcclusionQuery = nullptr;

	ID3D11Buffer* m_pLensflareCB = nullptr;

	// State
	ID3D11DepthStencilState* m_pNoDepthState = nullptr;
	ID3D11BlendState* m_pAddativeBlendState = nullptr;
	ID3D11BlendState* m_pNoColorBlendState = nullptr;

	Vec3 m_vSunWorldPos;
	Vec2 m_vSunPos2D;
	_float m_fSunVisibility = 0.0f;
	_bool m_bQuerySunVisibility = true;

	static const _int m_iTotalLights = 1;
	static const _int m_iTotalCoronaFlares = 4;
	static const _int m_iTotalFlares = 10;

	struct FLARE
	{
		_float fOffset = 0.0f;
		_float fScale = 0.0f;
		Vec4 vColor;
	};
	FLARE m_arrFlares[m_iTotalFlares] = {};

	_float m_fCoronaRotation = 0.0f;

	ID3D11BlendState* m_pPrevBlendState = nullptr;
	_float m_fPrevBlendFactor[4] = {};
	_uint m_iPrevSampleMask = 0;

public:
	static CVIBuffer_LensFlare* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END

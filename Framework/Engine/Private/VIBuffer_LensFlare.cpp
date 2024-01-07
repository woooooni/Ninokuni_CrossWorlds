#include "..\Public\VIBuffer_LensFlare.h"
#include "GameInstance.h"


CVIBuffer_LensFlare::CVIBuffer_LensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_LensFlare::CVIBuffer_LensFlare(const CVIBuffer_LensFlare& rhs)
	: CVIBuffer(rhs),
	m_pLensflareCB(rhs.m_pLensflareCB),
	m_pPredicate(rhs.m_pPredicate),
	m_pOcclusionQuery(rhs.m_pOcclusionQuery),
	m_pNoDepthState(rhs.m_pNoDepthState),
	m_pAddativeBlendState(rhs.m_pAddativeBlendState),
	m_pNoColorBlendState(rhs.m_pNoColorBlendState)
{
	Safe_AddRef<ID3D11Buffer*>(m_pLensflareCB);
	Safe_AddRef<ID3D11Predicate*>(m_pPredicate);
	Safe_AddRef<ID3D11Query*>(m_pOcclusionQuery);
	Safe_AddRef<ID3D11DepthStencilState*>(m_pNoDepthState);
	Safe_AddRef<ID3D11BlendState*>(m_pAddativeBlendState);
	Safe_AddRef<ID3D11BlendState*>(m_pNoColorBlendState);
	Safe_AddRef<ID3D11BlendState*>(m_pPrevBlendState);
}

HRESULT CVIBuffer_LensFlare::Initialize_Prototype()
{
	D3D11_QUERY_DESC queryDesc;
	::ZeroMemory(&queryDesc, sizeof(D3D11_QUERY_DESC));
	queryDesc.Query = D3D11_QUERY_OCCLUSION_PREDICATE;
	queryDesc.MiscFlags = 0;
	if (FAILED(m_pDevice->CreatePredicate(&queryDesc, &m_pPredicate)))
		return E_FAIL;

	queryDesc.Query = D3D11_QUERY_OCCLUSION;
	if (FAILED(m_pDevice->CreateQuery(&queryDesc, &m_pOcclusionQuery)))
		return E_FAIL;

	D3D11_BUFFER_DESC cbDesc;
	::ZeroMemory(&cbDesc, sizeof(&cbDesc));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.ByteWidth = ::CVIBuffer_LensFlare::m_iTotalFlares * sizeof(CB_LENSFLARE_VS);
	if (FAILED(m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pLensflareCB)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = false;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = false;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC defaultStencilOp = 
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace = defaultStencilOp;
	descDepth.BackFace = defaultStencilOp;
	if (FAILED(m_pDevice->CreateDepthStencilState(&descDepth, &m_pNoDepthState)))
		return E_FAIL;

	D3D11_BLEND_DESC descBlend;
	::ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.AlphaToCoverageEnable = false;
	descBlend.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC TRBlenddDesc =
	{
		true,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_ALL,
	};
	for (_uint i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		descBlend.RenderTarget[i] = TRBlenddDesc;

	if (FAILED(m_pDevice->CreateBlendState(&descBlend, &m_pAddativeBlendState)))
		return E_FAIL;

	TRBlenddDesc.BlendEnable = false;
	TRBlenddDesc.RenderTargetWriteMask = 0;
	for (_uint i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		descBlend.RenderTarget[i] = TRBlenddDesc;

	if (FAILED(m_pDevice->CreateBlendState(&descBlend, &m_pNoColorBlendState)))
		return E_FAIL;

	m_arrFlares[0].fOffset = 0.0f;
	m_arrFlares[0].fScale = 0.028f;
	m_arrFlares[0].vColor = Vec4(0.2f, 0.18f, 0.15f, 0.25f);
	m_arrFlares[1].fOffset = 0.0f;
	m_arrFlares[1].fScale = 0.028f;
	m_arrFlares[1].vColor = Vec4(0.2f, 0.18f, 0.15f, 0.25f);
	m_arrFlares[2].fOffset = 0.0f;
	m_arrFlares[2].fScale = 0.028f;
	m_arrFlares[2].vColor = Vec4(0.2f, 0.18f, 0.15f, 0.25f);
	m_arrFlares[3].fOffset = 0.0f;
	m_arrFlares[3].fScale = 0.028f;
	m_arrFlares[3].vColor = Vec4(0.2f, 0.18f, 0.15f, 0.25f);
	m_arrFlares[4].fOffset = 0.5f;
	m_arrFlares[4].fScale = 0.075f;
	m_arrFlares[4].vColor = Vec4(0.2f, 0.3f, 0.55f, 1.0f);
	m_arrFlares[5].fOffset = 1.0f;
	m_arrFlares[5].fScale = 0.054f;
	m_arrFlares[5].vColor = Vec4(0.024f, 0.2f, 0.52f, 1.0f);
	m_arrFlares[6].fOffset = 1.35f;
	m_arrFlares[6].fScale = 0.095f;
	m_arrFlares[6].vColor = Vec4(0.032f, 0.1f, 0.5f, 1.0f);
	m_arrFlares[7].fOffset = 0.9f;
	m_arrFlares[7].fScale = 0.065f;
	m_arrFlares[7].vColor = Vec4(0.13f, 0.14f, 0.58f, 1.0f);
	m_arrFlares[8].fOffset = 1.55f;
	m_arrFlares[8].fScale = 0.038f;
	m_arrFlares[8].vColor = Vec4(0.16f, 0.21, 0.44, 1.0f);
	m_arrFlares[9].fOffset = 0.25f;
	m_arrFlares[9].fScale = 0.1f;
	m_arrFlares[9].vColor = Vec4(0.23f, 0.21, 0.44, 0.85f);

	return S_OK;
}

HRESULT CVIBuffer_LensFlare::Initialize()
{
	return S_OK;
}

void CVIBuffer_LensFlare::Tick(const Vec3& sunWorldPos, _float fTimeDelta)
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	Matrix view = pPipeLine->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	Matrix proj = pPipeLine->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
	Matrix viewProj = view * proj;

	for (_int i = 0; i < m_iTotalLights; ++i)
	{
		m_vSunWorldPos = sunWorldPos;

		Vec3 ProjPos;
		ProjPos = ::XMVector3TransformCoord(m_vSunWorldPos, viewProj);
		m_vSunPos2D.x = ProjPos.x;
		m_vSunPos2D.y = ProjPos.y;
	}

	const _float fCoronaRotationSpeed = 0.01f * XM_PI;
	m_fCoronaRotation += fTimeDelta * fCoronaRotationSpeed;
}

HRESULT CVIBuffer_LensFlare::LensFlareRender()
{
	m_pContext->SetPredication(m_pPredicate, false);

	m_bQuerySunVisibility = false;
	UINT64 sunVisibility;

	if (m_pContext->GetData(m_pOcclusionQuery,
		static_cast<void*>(&sunVisibility), sizeof(sunVisibility), 0) == S_OK)
	{
		m_fSunVisibility = static_cast<_float>(sunVisibility) / 700.0f;
		m_bQuerySunVisibility = true;
	}

	ID3D11DepthStencilState* pPrevDepthState;
	m_pContext->OMGetDepthStencilState(&pPrevDepthState, nullptr);
	m_pContext->OMSetDepthStencilState(m_pNoDepthState, 0);

	ID3D11BlendState* pPrevBlendState;
	_float prevBlendFactor[4];
	_uint prevSampleMask;

	m_pContext->OMGetBlendState(&pPrevBlendState, prevBlendFactor, &prevSampleMask);
	m_pContext->OMSetBlendState(m_pAddativeBlendState, prevBlendFactor, prevSampleMask);

	m_pContext->IASetInputLayout(nullptr);
	m_pContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(m_pContext->Map(m_pLensflareCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return E_FAIL;

	CB_LENSFLARE_VS* pCB = static_cast<CB_LENSFLARE_VS*>(MappedResource.pData);
	for (_int j = 0; j < m_iTotalCoronaFlares; ++j)
	{
		pCB[j].Position = Vec4(m_vSunPos2D.x, m_vSunPos2D.y, 0.0f, 0.0f);

		_float fSin = 
			::sinf(m_fCoronaRotation + static_cast<_float>(j) * XM_PI * 0.25f + XM_PI * 0.2f);
		_float fCos =
			::cosf(m_fCoronaRotation + static_cast<_float>(j) * XM_PI * 0.25f + XM_PI * 0.2f);

		const _float coronaWidthScale = 14.0f;
		_float fScaleX = m_arrFlares[j].fScale * coronaWidthScale;
		_float fScaleY = m_arrFlares[j].fScale;

		_float fAspectRatio = 1600.0f / 900.0f;

		pCB[j].ScaleRotate = 
			Vec4(fScaleX * fCos, fScaleY * -fSin, fScaleX * fSin * fAspectRatio, fScaleY * fCos * fAspectRatio);
	
		pCB[j].Color = m_arrFlares[j].vColor * m_fSunVisibility;
	}
	m_pContext->Unmap(m_pLensflareCB, 0);

	return S_OK;
}

void CVIBuffer_LensFlare::BeginSunVisibility()
{
}

void CVIBuffer_LensFlare::EndSunVisiBility()
{
}

CVIBuffer_LensFlare* CVIBuffer_LensFlare::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CComponent* CVIBuffer_LensFlare::Clone(void* pArg)
{
	return nullptr;
}

void CVIBuffer_LensFlare::Free()
{
	__super::Free();

	Safe_Release<ID3D11Buffer*>(m_pLensflareCB);
	Safe_Release<ID3D11Predicate*>(m_pPredicate);
	Safe_Release<ID3D11Query*>(m_pOcclusionQuery);
	Safe_Release<ID3D11DepthStencilState*>(m_pNoDepthState);
	Safe_Release<ID3D11BlendState*>(m_pAddativeBlendState);
	Safe_Release<ID3D11BlendState*>(m_pNoColorBlendState);
	Safe_Release<ID3D11BlendState*>(m_pPrevBlendState);
}

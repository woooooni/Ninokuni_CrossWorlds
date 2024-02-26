#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"
#include "CascadeMatrixSet.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{

}



HRESULT CTarget_Manager::Reserve_Manager(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY)
{
	if (FAILED(Ready_Shadow_DSV(pDevice, iWinSizeX, iWinSizeY)))
		return E_FAIL;
	if (FAILED(Ready_Minimap_DSV(pDevice, iWinSizeX, iWinSizeY)))
		return E_FAIL;
	if (FAILED(Ready_UI_DSV(pDevice, iWinSizeX, iWinSizeY)))
		return E_FAIL;


	if (FAILED(Ready_Cascade_Initialize(pDevice)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTarget_Manager::Ready_Shadow_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY)
{
	if (nullptr == pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDSV = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinSizeX * 3.f;
	TextureDesc.Height = iWinSizeY * 3.f;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDSV)))
		return E_FAIL;

	if (FAILED(pDevice->CreateDepthStencilView(pDSV, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Ready_Minimap_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY)
{
	if (nullptr == pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDSV = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinSizeX;
	TextureDesc.Height = iWinSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDSV)))
		return E_FAIL;

	if (FAILED(pDevice->CreateDepthStencilView(pDSV, nullptr, &m_pMinimapDSV)))
		return E_FAIL;

	Safe_Release(pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Ready_UI_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY)
{
	if (nullptr == pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDSV = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width  = iWinSizeX;
	TextureDesc.Height = iWinSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count   = 1;

	TextureDesc.Usage     = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDSV)))
		return E_FAIL;

	if (FAILED(pDevice->CreateDepthStencilView(pDSV, nullptr, &m_pUIDSV)))
		return E_FAIL;

	Safe_Release(pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Ready_Cascade_Initialize(ID3D11Device* pDevice)
{
	const int iShadowMapSize = 2048;

	D3D11_TEXTURE2D_DESC dtd = {
		iShadowMapSize,
		iShadowMapSize,
		1,
		3,
		DXGI_FORMAT_R32_TYPELESS,
		1,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView =
	{
		DXGI_FORMAT_D32_FLOAT,
		D3D11_DSV_DIMENSION_TEXTURE2DARRAY,
		0
	};
	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderView =
	{
		DXGI_FORMAT_R32_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURE2DARRAY,
		0,
		0
	};

	if (FAILED(pDevice->CreateTexture2D(&dtd, nullptr, &m_pCascadeDepthStencilRT)))
		return E_FAIL;

	descDepthView.Texture2DArray.ArraySize = 3;
	if (FAILED(pDevice->CreateDepthStencilView(m_pCascadeDepthStencilRT, &descDepthView, &m_pCascadeDepthStencilDSV)))
		return E_FAIL;

	descShaderView.Texture2DArray.FirstArraySlice = 0;
	descShaderView.Texture2DArray.ArraySize = 3;
	descShaderView.Texture2DArray.MipLevels = 1;
	if (FAILED(pDevice->CreateShaderResourceView(m_pCascadeDepthStencilRT, &descShaderView, &m_pCascadeDepthStencilSRV)))
		return E_FAIL;

	m_pCascadeMatrixSet = new CCascadeMatrixSet();
	m_pCascadeMatrixSet->Init(iShadowMapSize);

	return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vColor, _uint iArraySize)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget*		pRenderTarget = CRenderTarget::Create(pDevice, pContext, iSizeX, iSizeY, ePixelFormat, vColor, iArraySize);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const wstring & strMRTTag, const wstring & strTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*	pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return	S_OK;
}

HRESULT CTarget_Manager::Bind_SRV(CShader * pShader, const wstring & strTargetTag, const _char * pConstantName)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_SRV(pShader, pConstantName);
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pContext, const wstring & strMRTTag, _bool bClear)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;
		
	pContext->OMGetRenderTargets(8, m_pPrevRTVs, &m_pDSV);

	ID3D11RenderTargetView*		pRenderTargets[8] = {};

	_uint			iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumRTVs++] = pRenderTarget->Get_RTV();
		if(bClear)
			pRenderTarget->Clear();
	}

	pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, m_pDSV);

	return	S_OK;
}

HRESULT CTarget_Manager::Begin_Minimap_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, _bool bClear)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	pContext->OMGetRenderTargets(8, m_pPrevRTVs, &m_pDSV);

	ID3D11RenderTargetView* pRenderTargets[8] = {};

	_uint			iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumRTVs++] = pRenderTarget->Get_RTV();
		if (bClear)
			pRenderTarget->Clear();
	}

	pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, m_pMinimapDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_Shadow_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag)
{
	D3D11_VIEWPORT	ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = 1600.f * 3.f;
	ViewPortDesc.Height = 900.f * 3.f;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;
	pContext->RSSetViewports(1, &ViewPortDesc);

	pContext->ClearDepthStencilView(m_pShadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	pContext->OMGetRenderTargets(8, m_pPrevRTVs, &m_pDSV);

	ID3D11RenderTargetView* pRenderTargets[8] = {};

	_uint			iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumRTVs++] = pRenderTarget->Get_RTV();
		pRenderTarget->Clear();
	}

	pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, m_pShadowDSV);

	return	S_OK;
}

HRESULT CTarget_Manager::Begin_UI_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, _bool bClear, _bool bStencil)
{
	if(true == bStencil)
		pContext->ClearDepthStencilView(m_pUIDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	pContext->OMGetRenderTargets(8, m_pPrevRTVs, &m_pDSV);

	ID3D11RenderTargetView* pRenderTargets[8] = {};

	_uint			iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumRTVs++] = pRenderTarget->Get_RTV();
		if (bClear)
			pRenderTarget->Clear();
	}


	pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, m_pUIDSV);

	return	S_OK;
}

//HRESULT CTarget_Manager::Begin_CascadeShadow_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag)
//{
//	D3D11_VIEWPORT	ViewPortDesc;
//	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
//	ViewPortDesc.TopLeftX = 0;
//	ViewPortDesc.TopLeftY = 0;
//	ViewPortDesc.Width = 1600.f * 3.f;
//	ViewPortDesc.Height = 900.f;
//	ViewPortDesc.MinDepth = 0.f;
//	ViewPortDesc.MaxDepth = 1.f;
//	pContext->RSSetViewports(1, &ViewPortDesc);
//
//	pContext->ClearDepthStencilView(m_pCascadeDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
//
//	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
//
//	if (nullptr == pMRTList)
//		return E_FAIL;
//
//	pContext->OMGetRenderTargets(8, m_pPrevRTVs, &m_pDSV);
//
//	ID3D11RenderTargetView* pRenderTargets[8] = {};
//
//	_uint			iNumRTVs = 0;
//
//	for (auto& pRenderTarget : *pMRTList)
//	{
//		pRenderTargets[iNumRTVs++] = pRenderTarget->Get_RTV();
//		pRenderTarget->Clear();
//	}
//
//	pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, m_pCascadeDSV);
//
//	return	S_OK;
//}

HRESULT CTarget_Manager::Begin_Cascade_MRT(ID3D11DeviceContext* pContext, const Vec3& vDirectionalDir)
{
	const int iShadowMapSize = 2048;

	D3D11_VIEWPORT vp[3] =
	{ { 0, 0, iShadowMapSize, iShadowMapSize, 0.0f, 1.0f },
		{ 0, 0, iShadowMapSize, iShadowMapSize, 0.0f, 1.0f },
		{ 0, 0, iShadowMapSize, iShadowMapSize, 0.0f, 1.0f } };

	pContext->RSSetViewports(1, vp);
	
	pContext->ClearDepthStencilView(m_pCascadeDepthStencilDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	ID3D11RenderTargetView* nullRT = nullptr;
	pContext->OMSetRenderTargets(1, &nullRT, m_pCascadeDepthStencilDSV);

	// ��Ʈ���� ������Ʈ
	//m_pCascadeMatrixSet->Tick(vDirectionalDir);
	//m_pCascadeMatrixSet->SubdivisionFrustum(vDirectionalDir);
	m_pCascadeMatrixSet->FrustumUpdate();
	//m_pCascadeMatrixSet->UpdateShadowMatrix(vDirectionalDir);
	return S_OK;
}

HRESULT CTarget_Manager::End_Cascade_MRT(ID3D11DeviceContext* pContext)
{
	pContext->OMSetRenderTargets(8, m_pPrevRTVs, m_pDSV);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = 1600.f;
	ViewPortDesc.Height = 900.f;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CTarget_Manager::Bind_Cascade_SRV(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Texture(pConstantName, m_pCascadeDepthStencilSRV);
}

HRESULT CTarget_Manager::Clear_RenderTarget(const wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;
	
	return pRenderTarget->Clear();
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pContext)
{

	pContext->OMSetRenderTargets(8, m_pPrevRTVs, m_pDSV);

	for (_uint i = 0; i < 8; ++i)	
		Safe_Release(m_pPrevRTVs[i]);

	Safe_Release(m_pDSV);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = 1600.f;
	ViewPortDesc.Height = 900.f;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	pContext->RSSetViewports(1, &ViewPortDesc);

	return	S_OK;
}


ID3D11Texture2D* CTarget_Manager::Get_Texture_FromRenderTarget(const wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return nullptr;

	
	return pRenderTarget->Get_Texture();
}

//#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_Debug(const wstring & strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(strTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);	
}

HRESULT CTarget_Manager::Render(const wstring & strMRTTag, CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

//#endif


CRenderTarget * CTarget_Manager::Find_RenderTarget(const wstring & strTargetTag)
{
	auto	iter = m_RenderTargets.find(strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const wstring & strMRTTag)
{
	auto	iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);

		Pair.second.clear();
	}
	m_MRTs.clear();
		

	for (auto& Pair : m_RenderTargets)	
		Safe_Release(Pair.second);

	m_RenderTargets.clear(); 

	m_pDSV = nullptr;
	Safe_Release(m_pMinimapDSV);
	Safe_Release(m_pShadowDSV);
	Safe_Release(m_pUIDSV);

	Safe_Release<ID3D11Texture2D*>(m_pCascadeDepthStencilRT);
	Safe_Release<ID3D11DepthStencilView*>(m_pCascadeDepthStencilDSV);
	Safe_Release<ID3D11ShaderResourceView*>(m_pCascadeDepthStencilSRV);
	Safe_Release<CCascadeMatrixSet*>(m_pCascadeMatrixSet);
}

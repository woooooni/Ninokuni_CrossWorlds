#include "stdafx.h"
#include "..\Public\PerlinFire.h"
#include "GameInstance.h"
#include "Quest_Manager.h"

CPerlinFire::CPerlinFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CPerlinFire::CPerlinFire(const CPerlinFire& rhs)
	: CGameObject(rhs)
	, m_pNoiseTexture(rhs.m_pNoiseTexture)
{
	for (_uint i = 0; i < TEX_TYPE::TEX_END; ++i)
	{
		m_pSRV[i] = rhs.m_pSRV[i];
		Safe_AddRef<ID3D11ShaderResourceView*>(m_pSRV[i]);
	}

	Safe_AddRef<ID3D11Texture2D*>(m_pNoiseTexture);
}

HRESULT CPerlinFire::Initialize_Prototype()
{
	Prepare_Texture();

	return S_OK;
}

HRESULT CPerlinFire::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CPerlinFire::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime = fTimeDelta;
}

void CPerlinFire::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
}

HRESULT CPerlinFire::Render()
{
	Matrix world, view, Projection;
	world = m_pTransformCom->Get_WorldMatrix();
	view = GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	Projection = GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);


	Matrix WorldView, WorldViewProj;
	WorldView = world * view;
	WorldViewProj = WorldView * Projection;

	XMMATRIX mTranslate, mScale, mWorldViewInv;
	mTranslate = ::XMMatrixTranslation(0.0f, 0.5f, 0.0f);
	mScale = ::XMMatrixScaling(12.0f, 24.0f, 12.0f);

	XMMATRIX CalcuWorld = m_pTransformCom->Get_WorldMatrix();
	XMMATRIX CalcuView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);

	Matrix mWorldView = mTranslate * mScale * CalcuWorld * CalcuView;
	mWorldViewInv = ::XMMatrixInverse(nullptr, mWorldView);

	Vec4 vEye;
	Vec4 v(0.0f, 0.0f, 0.0f, 1.0f);
	vEye = ::XMVector4Transform(v, mWorldViewInv);

	if (FAILED(m_pShaderCom->Bind_Matrix("WorldViewProj", &WorldViewProj)))
		return E_FAIL;

	const Vec4 vBloomColor = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
	if (FAILED(m_pShaderCom->Bind_RawValue("vBloomColor", &vBloomColor, sizeof(Vec4))))
		return E_FAIL;

	// Perlin Fire
	if (FAILED(m_pShaderCom->Bind_Texture("g_JitterTexture", m_pSRV[TEX_TYPE::JITTER_TEX])))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Texture("g_PermTexture", m_pSRV[TEX_TYPE::PERM_TEX])))
		return E_FAIL;
	if (FAILED(GI->Bind_SRV(m_pShaderCom, TEXT("Target_Depth"), "g_ScreenDepth")))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FireShape", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("EyePos", &vEye, sizeof(Vec4))))
		return E_FAIL;
	if(FAILED(m_pShaderCom->Bind_RawValue("Time", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("bJitter", &m_bJitter, sizeof(_bool))))
		return E_FAIL;
	_float stepSize = 1.0f / static_cast<_float>(m_iSampleingRate);
	if (FAILED(m_pShaderCom->Bind_RawValue("StepSize", &stepSize, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("NoiseScale", &m_fNoiseScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("Roughness", &m_fRoughness, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("FrequencyWeights", &m_fFrequencyWeights, sizeof(_float) * 5)))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Begin(8)))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPerlinFire::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FireTex"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPerlinFire::Prepare_Texture()
{
	srand((_uint)time(NULL));

	BYTE* data = new BYTE[256 * 256];

	for (int i = 0; i < 256 * 256; i++)
		data[i] = static_cast<unsigned char>(rand() % 256);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 256;
	desc.Height = 256;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA dataDesc;
	dataDesc.pSysMem = data;
	dataDesc.SysMemPitch = 256;

	if (FAILED(m_pDevice->CreateTexture2D(&desc, &dataDesc, &m_pNoiseTexture)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	::ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = DXGI_FORMAT_R8_UNORM;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	descSRV.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pNoiseTexture, &descSRV, &m_pSRV[TEX_TYPE::JITTER_TEX])))
		return E_FAIL;

	descSRV.Format = DXGI_FORMAT_R8_UINT;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pNoiseTexture, &descSRV, &m_pSRV[TEX_TYPE::PERM_TEX])))
		return E_FAIL;

	Safe_Delete_Array<BYTE*>(data);

	return S_OK;
}

CPerlinFire* CPerlinFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CPerlinFire* pInstance = new CPerlinFire(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPerlinFire");
		Safe_Release<CPerlinFire*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPerlinFire::Clone(void* pArg)
{
	CPerlinFire* pInstance = new CPerlinFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPerlinFire");
		Safe_Release<CPerlinFire*>(pInstance);
	}

	return pInstance;
}

void CPerlinFire::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIBufferCom);

	for (_uint i = 0; i < TEX_TYPE::TEX_END; ++i)
		Safe_Release<ID3D11ShaderResourceView*>(m_pSRV[i]);

	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<ID3D11Texture2D*>(m_pNoiseTexture);
}

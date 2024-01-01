#include "..\Public\EvermoreWater.h"
#include "GameInstance.h"

CEvermoreWater::CEvermoreWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Evermore_Water", OBJ_TYPE::OBJ_DYNAMIC)
{
}

CEvermoreWater::CEvermoreWater(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEvermoreWater::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CEvermoreWater::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_vDirection.x = GI->RandomFloat(-1.0f, 1.0f);
	m_vDirection.y = GI->RandomFloat(-1.0f, 1.0f);

	return S_OK;
}

void CEvermoreWater::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime += fTimeDelta;

	if (m_fTime >= FLT_MAX - 0.05f)
		m_fTime = 0.0f;
}

void CEvermoreWater::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
}

HRESULT CEvermoreWater::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEvermoreWater::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Water"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Entire_Water"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Ocean_Terrain"),
	//	TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	//	return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NORMAL]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NOISE1]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NOISE2]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Diffuse"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CEvermoreWater::Bind_ShaderResources()
{

	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("World", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("View", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("Projection", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	Vec4 vCameraPos = GI->Get_CamPosition();

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCameraPos, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NORMAL]->Bind_ShaderResource(m_pShaderCom, "NormalTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NOISE1]->Bind_ShaderResource(m_pShaderCom, "MaskTexture", 69)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NOISE2]->Bind_ShaderResource(m_pShaderCom, "MaskTexture2", 158)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("fWaterTranslationSpeed", &m_fWaterTranslationSpeed, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("fWaterTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("fReflectRefractScale", &m_fReflectRefractSacle, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("bFresnel", &m_bFresnel, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("flowDirection", &m_vDirection, sizeof(Vec2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("fBloomTiling", &m_fBloomTiling, sizeof(_float))))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, 0)))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Begin(0)))
	//	return E_FAIL;
	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;

#pragma region Wave
	//if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "World")))
	//	return E_FAIL;
	//if (FAILED(GI->Bind_TransformToShader(m_pShaderCom, "View", CPipeLine::D3DTS_VIEW)))
	//	return E_FAIL;
	//if (FAILED(GI->Bind_TransformToShader(m_pShaderCom, "Projection", CPipeLine::D3DTS_PROJ)))
	//	return E_FAIL;

	//Vec4 vCamPos = GI->Get_CamPosition();

	//if (FAILED(m_pShaderCom->Bind_RawValue("vCameraPosition", &vCamPos, sizeof(Vec4))))
		//return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("VS_Ocean", &m_vsOceanBuffer, sizeof(VS_OceanBuffer))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("PS_Ocean", &m_psOceanBuffer, sizeof(PS_OceanBuffer))))
	//	return E_FAIL;

#pragma region Vertex
	//if (FAILED(m_pShaderCom->Bind_RawValue("fWaveFrequency", &m_vsOceanBuffer.fWaveFrequency, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fWaveAmplitude", &m_vsOceanBuffer.fWaveFrequency, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fBumpHeight", &m_vsOceanBuffer.fWaveFrequency, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("vBumpSpeed", &m_vsOceanBuffer.fWaveFrequency, sizeof(Vec2))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("vBumpSpeed", &m_vsOceanBuffer.fTextureScale, sizeof(Vec2))))
	//	return E_FAIL;
#pragma endregion Vertex


#pragma region Pixel
	//if (FAILED(m_pShaderCom->Bind_RawValue("fFresnelBias", &m_psOceanBuffer.fFresnelBias, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fFresnelPower", &m_psOceanBuffer.fFresnelPower, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fFresnelAmount", &m_psOceanBuffer.fFresnelAmount, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fShoreBlend", &m_psOceanBuffer.fShoreBlend, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fHeightRatio", &m_psOceanBuffer.fHeightRatio, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("vOceanSize", &m_psOceanBuffer.vOceanSize, sizeof(Vec2))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("vShallowColor", &m_psOceanBuffer.vOceanSize, sizeof(Vec4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("vDeepColor", &m_psOceanBuffer.vOceanSize, sizeof(Vec4))))
	//	return E_FAIL;
#pragma endregion Pixel

	//if(FAILED(m_pShaderCom->Bind_RawValue("fTime", &m_fTime, sizeof(_float))))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NORMAL]->Bind_ShaderResource(m_pShaderCom, "NormalMap")))
	//	return E_FAIL;
	//if (FAILED(m_pTextureCom[TEX_TYPE::TE]->Bind_ShaderResource(m_pShaderCom, "HeightMap", 0)))
	//	return E_FAIL;
	//if (FAILED(m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "DiffuseMap", 0)))
	//	return E_FAIL;

	//const LIGHTDESC* lightDesc = GI->Get_LightDesc(0);
	//Vec4 vAmbient = lightDesc->vAmbient;

	//if (FAILED(m_pShaderCom->Bind_RawValue("vSunAmbient", &vAmbient, sizeof(Vec4))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Begin(0)))
	//	return E_FAIL;
	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;
#pragma endregion Wave

	return S_OK;
}

CEvermoreWater* CEvermoreWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEvermoreWater* pInstance = new CEvermoreWater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEvermoreWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEvermoreWater::Clone(void* pArg)
{
	CEvermoreWater* pInstance = new CEvermoreWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEvermoreWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvermoreWater::Free()
{
	__super::Free();

	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CShader*>(m_pShaderCom);

	for (_uint i = 0; i < TEX_TYPE::TEX_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);

	Safe_Release<CModel*>(m_pModelCom);
	//Safe_Release<CVIBuffer_Ocean*>(m_pVIBufferCom);
}

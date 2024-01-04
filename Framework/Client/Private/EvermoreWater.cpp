#include "..\Public\EvermoreWater.h"
#include "GameInstance.h"
#include "Light.h"

CEvermoreWater::CEvermoreWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWater(pDevice, pContext, m_strObjectTag, OBJ_TYPE::OBJ_WATER)
{
	m_strObjectTag = TEXT("Everemore_Water");
}

CEvermoreWater::CEvermoreWater(const CWater& rhs)
	: CWater(rhs)
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Ocean"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Everemore_Water"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Wave_Normal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NORMAL]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_CubeMap"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]))))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
	//	TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NOISE1]))))
	//	return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
	//	TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NOISE2]))))
	//	return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Diffuse"),
	//	TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]))))
	//	return E_FAIL;

	
	return S_OK;
}

HRESULT CEvermoreWater::Bind_ShaderResources()
{
	//if (nullptr == m_pModelCom)
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("World", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("View", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("Projection", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	Vec4 vCameraPos = GI->Get_CamPosition();

	list<CLight*>* lightList = GI->Get_LightList();
	if (nullptr != lightList)
	{
		for (auto& pLight : *lightList)
		{
			if (pLight->Get_LightDesc()->eType != LIGHTDESC::TYPE_DIRECTIONAL)
				continue;

			m_PSGerstnerWave.vLightColor = pLight->Get_LightDesc()->vDiffuse;
			m_PSGerstnerWave.vLightDir = Vec3(
				pLight->Get_LightDesc()->vDirection.x, 
				pLight->Get_LightDesc()->vDirection.y, 
				pLight->Get_LightDesc()->vDirection.z);
		}
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("vCameraPosition", &vCameraPos, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("VS_Gerstner", &m_VSGerstnerWave, sizeof(VS_GerstnerWave))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("PS_Gerstner", &m_PSGerstnerWave, sizeof(PS_GerstnerWave))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("time", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NORMAL]->Bind_ShaderResource(m_pShaderCom, "NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "Env")))
		return E_FAIL;

	//if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NOISE1]->Bind_ShaderResource(m_pShaderCom, "MaskTexture", 69)))
	//	return E_FAIL;
	//if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NOISE2]->Bind_ShaderResource(m_pShaderCom, "MaskTexture2", 158)))
	//	return E_FAIL;


	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	//_uint iPassIndex = 0;

	//for (_uint i = 0; i < iNumMeshes; ++i)
	//{
	//	if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "DiffuseTexture")))
	//		return E_FAIL;
	//}

	//if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, 1)))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


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

	//Safe_Release<CModel*>(m_pModelCom);
	Safe_Release<CVIBuffer_Terrain*>(m_pVIBufferCom);
}

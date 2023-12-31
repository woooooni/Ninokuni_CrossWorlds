#include "stdafx.h"
#include "..\Public\Water.h"
#include "GameInstance.h"
#include "Camera_Manager.h"

CWater::CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CDynamicObject(pDevice, pContext, strObjectTag, eType)
{

}

CWater::CWater(const CWater& rhs)
	: CDynamicObject(rhs)
{
}

HRESULT CWater::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_vDirection.x = GI->RandomFloat(-1.0f, 1.0f);
	m_vDirection.y = GI->RandomFloat(-1.0f, 1.0f);

	return S_OK;
}

void CWater::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > FLT_MAX)
		m_fTime = 0.0f;
}

void CWater::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
}

HRESULT CWater::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Water"),
		TEXT("Com_Water_Shader"), reinterpret_cast<CComponent**>(&m_pWaterShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Everemore_Water"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Diffuse"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pDiffuseTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom[0]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom[1]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr != m_pModelCom)
		Safe_Release<CModel*>(m_pModelCom);

	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	m_strDynamicName = szFileName;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, wstring(L"Prototype_Component_Model_") + szFileName,
		eType, strFilePath, strFileName)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWater::Bind_ShaderResources()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("World", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pWaterShaderCom->Bind_RawValue("View", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pWaterShaderCom->Bind_RawValue("Projection", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	Vec4 vCameraPos = GI->Get_CamPosition();

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("g_vCamPosition", &vCameraPos, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pWaterShaderCom, "NormalTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pNoiseTextureCom[0]->Bind_ShaderResource(m_pWaterShaderCom, "MaskTexture", 69)))
		return E_FAIL;
	if (FAILED(m_pNoiseTextureCom[1]->Bind_ShaderResource(m_pWaterShaderCom, "MaskTexture2", 149)))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("fWaterTranslationSpeed", &m_fWaterTranslationSpeed, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("fWaterTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("fReflectRefractScale", &m_fReflectRefractSacle, sizeof(_float))))
		return E_FAIL;

	if(FAILED(m_pWaterShaderCom->Bind_RawValue("bFresnel", &m_bFresnel, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("flowDirection", &m_vDirection, sizeof(Vec2))))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pWaterShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pModelCom->Render(m_pWaterShaderCom, 0, 0)))
		return E_FAIL;
	

	return S_OK;
}

CWater* CWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CWater* pInstance = new CWater(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Water");
		Safe_Release<CWater*>(pInstance);
	}

	return pInstance;
}

CGameObject* CWater::Clone(void* pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Water");
		Safe_Release<CWater*>(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pWaterShaderCom);
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CTexture*>(m_pDiffuseTextureCom);
	
	for(_uint i = 0; i < 2; ++i)
		Safe_Release<CTexture*>(m_pNoiseTextureCom[i]);
}

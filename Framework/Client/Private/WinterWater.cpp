#include "stdafx.h"
#include "..\Public\WinterWater.h"
#include "GameInstance.h"
#include "Camera_Manager.h"

CWinterWater::CWinterWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CDynamicObject(pDevice, pContext, strObjectTag, eType)
{

}

CWinterWater::CWinterWater(const CWinterWater& rhs)
	: CDynamicObject(rhs)
{
}

HRESULT CWinterWater::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWinterWater::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_vDirection.x = GI->RandomFloat(-1.0f, 1.0f);
	m_vDirection.y = GI->RandomFloat(-1.0f, 1.0f);

	return S_OK;
}

void CWinterWater::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > FLT_MAX)
		m_fTime = 0.0f;
}

void CWinterWater::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
}

HRESULT CWinterWater::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinterWater::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Water_Shader"), reinterpret_cast<CComponent**>(&m_pWaterShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Entire_Water"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_NORMAL]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Water_Diffuse"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_MASK1]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::TEX_MASK2]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinterWater::Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName)
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

HRESULT CWinterWater::Bind_ShaderResources()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pWaterShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pWaterShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	Vec4 vCameraPos = GI->Get_CamPosition();

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("g_vCamPosition", &vCameraPos, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_NORMAL]->Bind_ShaderResource(m_pWaterShaderCom, "g_NormalTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_MASK1]->Bind_ShaderResource(m_pWaterShaderCom, "g_MaskMap", 69)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_MASK2]->Bind_ShaderResource(m_pWaterShaderCom, "g_MaskMap2", 149)))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("fWaterTranslationSpeed", &m_fWaterTranslationSpeed, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("fWaterTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pWaterShaderCom->Bind_RawValue("flowDirection", &m_vDirection, sizeof(Vec2))))
		return E_FAIL;


	if (FAILED(m_pTextureCom[TEX_TYPE::TEX_DIFFUSE]->Bind_ShaderResource(m_pWaterShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	if (FAILED(m_pModelCom->Render(m_pWaterShaderCom, 0, 3)))
		return E_FAIL;

	return S_OK;
}

CWinterWater* CWinterWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CWinterWater* pInstance = new CWinterWater(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Water");
		Safe_Release<CWinterWater*>(pInstance);
	}

	return pInstance;
}

CGameObject* CWinterWater::Clone(void* pArg)
{
	CWinterWater* pInstance = new CWinterWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Water");
		Safe_Release<CWinterWater*>(pInstance);
	}

	return pInstance;
}

void CWinterWater::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pWaterShaderCom);

	for (_uint i = 0; i < TEX_TYPE::TEX_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);
}

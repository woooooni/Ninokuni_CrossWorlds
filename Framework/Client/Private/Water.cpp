#include "stdafx.h"
#include "..\Public\Water.h"
#include "GameInstance.h"

CWater::CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CDynamicObject(pDevice, pContext, strObjectTag, eType)
{

}

CWater::CWater(const CWater& rhs)
	: CDynamicObject(rhs)
{
}

HRESULT CWater::Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (FAILED(Ready_ModelCom(eType, strFilePath, strFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CWater::Tick(_float fTimeDelta)
{

}

void CWater::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
}

HRESULT CWater::Render()
{
	//if (nullptr == m_pModelCom)
	//	return E_FAIL;

	//CShader* pShader = m_pModelCom->Get_ModelType() == CModel::TYPE::TYPE_NONANIM ? m_pNonAnimShaderCom : m_pAnimShaderCom;

	//if (FAILED(pShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(pShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(pShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	//	return E_FAIL;

	//_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	//for (_uint i = 0; i < iNumMeshes; ++i)
	//{
	//	_uint		iPassIndex = 0;

	//	if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
	//		return E_FAIL;

	//	if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
	//		iPassIndex = 0;
	//	else
	//		iPassIndex++;

	//	if (FAILED(m_pModelCom->Render(pShader, i)))
	//		return E_FAIL;
	//}

	return S_OK;
}

HRESULT CWater::Ready_Components()
{
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

CWater* CWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
	const wstring& strFilePath, const wstring& strFileName, _int eObjType)
{
	CWater* pInstance = new CWater(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype(eObjType, strFilePath, strFileName)))
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
}

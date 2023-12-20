#include "stdafx.h"
#include "..\Public\StaticObject.h"
#include "GameInstance.h"

CStaticObject::CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
	
}

CStaticObject::CStaticObject(const CStaticObject& rhs)
	: CGameObject(rhs)
	, m_strMapObjName(rhs.m_strMapObjName)
{
}

HRESULT CStaticObject::Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_ModelCom(eType, strFilePath, strFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::Initialize(void* pArg)
{
	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;

	if (nullptr != pArg)
	{
		// TODO

	}

	return S_OK;
}

void CStaticObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CStaticObject::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CStaticObject::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CShader* pShader = m_pModelCom->Get_ModelType() == CModel::TYPE::TYPE_NONANIM ? m_pNonAnimShaderCom : m_pAnimShaderCom;

	if (FAILED(pShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	iPassIndex = 0;
		//else
		//	iPassIndex++;

		if (FAILED(m_pModelCom->Render(pShader, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStaticObject::Render_ShadowDepth()
{
	CShader* pShader = m_pModelCom->Get_ModelType() == CModel::TYPE::TYPE_NONANIM ? m_pNonAnimShaderCom : m_pAnimShaderCom;

	if (nullptr == pShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(0), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(pShader, i, 10)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CStaticObject::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

HRESULT CStaticObject::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

void CStaticObject::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CStaticObject::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CStaticObject::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CStaticObject::Ready_Components()
{
	return S_OK;
}

HRESULT CStaticObject::Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr != m_pModelCom)
		Safe_Release<CModel*>(m_pModelCom);

	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	m_strMapObjName = szFileName;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, wstring(L"Prototype_Component_Model_") + szFileName,
		eType, strFilePath, strFileName)))
		return E_FAIL;


	return S_OK;
}

void CStaticObject::YRotation(_float fSpeed, _float fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Turn(Vec4(0.0f, 1.0f, 0.0f, 1.0f), fSpeed, fTimeDelta);
}

void CStaticObject::XRotation(_float fSpeed, _float fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Turn(Vec4(1.0f, 0.0f, 0.0f, 1.0f), fSpeed, fTimeDelta);
}

void CStaticObject::ZRotation(_float fSpeed, _float fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Turn(Vec4(1.0f, 1.0f, 0.0f, 1.0f), fSpeed, fTimeDelta);
}

void CStaticObject::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pAnimShaderCom);
	Safe_Release<CShader*>(m_pNonAnimShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CModel*>(m_pModelCom);

	//CShader* m_pShaderCom = nullptr;
	//CRenderer* m_pRendererCom = nullptr;
	//CTransform* m_pTransformCom = nullptr;
	//CModel* m_pModelCom = nullptr;
}

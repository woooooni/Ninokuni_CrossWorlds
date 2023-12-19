#include "stdafx.h"
#include "..\Public\DynamicObject.h"
#include "GameInstance.h"

CDynamicObject::CDynamicObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int iObjectType)
	: CGameObject(pDevice, pContext, strObjectTag, iObjectType)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& rhs)
	: CGameObject(rhs)
	, m_strDynamicName(rhs.m_strDynamicName)
{
}

HRESULT CDynamicObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamicObject::Initialize(void* pArg)
{
	return S_OK;
}

void CDynamicObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CDynamicObject::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CDynamicObject::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pAnimShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	iPassIndex = 0;
		//else
		//	iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pAnimShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDynamicObject::Render_ShadowDepth()
{
	if (nullptr == m_pAnimShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pAnimShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pAnimShaderCom, m_pModelCom->Get_MaterialIndex(0), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pAnimShaderCom, i, 10)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CDynamicObject::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

HRESULT CDynamicObject::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

void CDynamicObject::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CDynamicObject::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CDynamicObject::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CDynamicObject::Ready_Components()
{
	return S_OK;
}

void CDynamicObject::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pAnimShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CModel*>(m_pModelCom);
}

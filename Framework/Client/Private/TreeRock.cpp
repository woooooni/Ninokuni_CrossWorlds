#include "stdafx.h"
#include "..\Public\TreeRock.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Quest_Manager.h"

CTreeRock::CTreeRock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CStaticObject(pDevice, pContext, strObjectTag, eType)
{
}

CTreeRock::CTreeRock(const CTreeRock& rhs)
	: CStaticObject(rhs)
{
}

HRESULT CTreeRock::Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (FAILED(__super::Initialize_Prototype(eType, strFilePath, strFileName)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTreeRock::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// Transform은 알아서 해야됨

	return S_OK;
}

void CTreeRock::Tick(_float fTimeDelta)
{
	_uint iCurLevel = GI->Get_CurrentLevel();


	if (true == m_bFirst && TEXT("Common_TreeB_02a") == m_strObjectTag && LEVELID::LEVEL_ICELAND == iCurLevel)
	{
		CEffect_Manager::GetInstance()->Generate_Decal_To_Position(TEXT("TreeSnowu"), m_pTransformCom->Get_WorldMatrix(), Vec3(0.0f, 12.0f, 0.0f), Vec3(5.0f, 7.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), this, &m_pDecalCom, false);	
		m_bFirst = false;
	}

	__super::Tick(fTimeDelta);
}

void CTreeRock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_BOSS_KILL)
	{
		Compute_CamZ(m_pTransformCom->Get_Position());

		if (m_fCamDistance <= 150.0f && true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 50.0f))
			m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	}
	else if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 50.f))
	{
		// Shadow 필요하면 ShadowRender 추가?
	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDERGROUP::RENDER_SHADOW, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
		m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	}


}

HRESULT CTreeRock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeRock::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeRock::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	Matrix worldInvTranspose = m_pTransformCom->Get_WorldMatrixInverse();
	worldInvTranspose.Transpose();

	Matrix worldInvTransposeView = worldInvTranspose * GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(pInstancingShader->Bind_Matrix("WorldInvTransposeView", &worldInvTransposeView)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint iPassIndex = 0;
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CTreeRock::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;
	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CTreeRock::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_NonAnim_Shader"), reinterpret_cast<CComponent**>(&m_pNonAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + m_strMapObjName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

CTreeRock* CTreeRock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
	const wstring& strFilePath, const wstring& strFileName, _int eObjType, _uint eModelType)
{
	CTreeRock* pInstance = new CTreeRock(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, strFilePath, strFileName)))
	{
		MSG_BOX("Create Failed to ProtoType : CTreeRock");
		Safe_Release<CTreeRock*>(pInstance);
	}

	return pInstance;
}

CGameObject* CTreeRock::Clone(void* pArg)
{
	CTreeRock* pInstance = new CTreeRock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CTreeRock");
		Safe_Release<CTreeRock*>(pInstance);
	}

	return pInstance;
}

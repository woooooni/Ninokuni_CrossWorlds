#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "..\Public\CurlingGame_Prop.h"

#include "Renderer.h"

CCurlingGame_Prop::CCurlingGame_Prop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CURLINGGAME_PROP)
{
}

CCurlingGame_Prop::CCurlingGame_Prop(const CCurlingGame_Prop& rhs)
	: CGameObject(rhs)
	, m_pModelCom(rhs.m_pModelCom)
	, m_pShaderCom(rhs.m_pShaderCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_pRigidBodyCom(rhs.m_pRigidBodyCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_AnimInstanceDesc(rhs.m_AnimInstanceDesc)
	, m_eCGType(rhs.m_eCGType)
	, m_iColliderDetectionType(rhs.m_iColliderDetectionType)
	, m_bActive(rhs.m_bActive)
{

}

HRESULT CCurlingGame_Prop::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Prop::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCurlingGame_Prop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pRigidBodyCom != nullptr)
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::CURLINGGAME_PROP, this);
}

void CCurlingGame_Prop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		//m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
		//m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
	}
	else
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		//m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
		//m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_SHADOW, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	}

	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
#ifdef _DEBUG
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
#endif
	}

	Compute_CamZ(m_pTransformCom->Get_Position());
}

HRESULT CCurlingGame_Prop::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	/* WVP */
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
	}

	/* Rim, Bloom */
	{
		_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
			return E_FAIL;
	}

	/* Models */
	{
		if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
		{
			if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
				return E_FAIL;
		}

		const _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Prop::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom || nullptr == m_pModelCom)
		return E_FAIL;

	/* WVP */
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
	}

	/* Models */
	if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
	{
		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;
	}

	/* Render */
	{
		const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Prop::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;

	/* WVP */
	{
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
	}

	/* Render */
	{
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			_uint iPassIndex = 0;
		
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
		
			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Prop::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;

	/* WVP */
	{
		_float4 vCamPosition = GI->Get_CamPosition();

		if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
			return E_FAIL;
		if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;

		Matrix worldInvTranspose = m_pTransformCom->Get_WorldMatrixInverse();
		worldInvTranspose.Transpose();

		Matrix worldInvTransposeView = worldInvTranspose * GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);

		if (FAILED(pInstancingShader->Bind_Matrix("WorldInvTransposeView", &worldInvTransposeView)))
			return E_FAIL;
	}

	/* Render */
	{
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
		
			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Prop::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	__super::Render();

	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;

	/* WVP */
	{
		if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
			return E_FAIL;
	}

	/* Models */
	{
		if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_AnimInstancingDesc", AnimModelDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * AnimModelDesc.size())))
			return E_FAIL;
	}


	/* Render */
	{
		_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
		_uint iPassIndex = 0;

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
				iPassIndex = 0;
			else
				iPassIndex++;

			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Prop::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;

	/* WVP */
	{
		if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
	}

	/* Models */
	{
		if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
			return E_FAIL;

		if (FAILED(pInstancingShader->Bind_RawValue("g_AnimInstancingDesc", AnimModelDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * AnimModelDesc.size())))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
			return E_FAIL;
	}

	/* Render */
	{
		_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCurlingGame_Prop::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CCurlingGame_Prop::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pTransformCom);
}

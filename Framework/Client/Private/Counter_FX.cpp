#include "stdafx.h"
#include "..\Public\Plants.h"
#include "GameInstance.h"
#include "..\Public\Counter_FX.h"

CCounter_FX::CCounter_FX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Counter_FX", OBJ_TYPE::OBJ_EFFECT)
{

}

CCounter_FX::CCounter_FX(const CCounter_FX& rhs)
	: CGameObject(rhs)
{

}


HRESULT CCounter_FX::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCounter_FX::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	COUNTER_FX_DESC* Desc = static_cast<COUNTER_FX_DESC*>(pArg);

	if (nullptr == Desc->pModel || nullptr == Desc->pRenderModel)
		return E_FAIL;

	m_WorldMatrix = Desc->WorldMatrix;
	m_vRimColor = Desc->vRimColor;
	m_vBloomPower = Desc->vBloomPower;
	m_fBlurPower = Desc->fBlurPower;

	m_pModelCom = Desc->pModel;
	m_pRenderModelCom = Desc->pRenderModel;

	Safe_AddRef(m_pModelCom);
	Safe_AddRef(m_pRenderModelCom);

	m_TweenDesc = Desc->TweenDesc;
	m_strPrototypeTag = m_pRenderModelCom->Get_Name() + L"Counter_FX";

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(m_WorldMatrix);
	m_vInitScale = m_pTransformCom->Get_Scale();

	return S_OK;
}

void CCounter_FX::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fAlpha -= m_fAlphaSpeed * fTimeDelta;
	if (m_fAlpha <= 0.f)
	{
		m_fAlpha = 0.f;
		Set_Dead(true);
		return;
	}
}

void CCounter_FX::LateTick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	__super::LateTick(fTimeDelta);

	// m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_TweenDesc);
	if(true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 5.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CCounter_FX::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &m_vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fMotionTrailAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlurPower", &m_fBlurPower, sizeof(_float))))
		return E_FAIL;

	if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
	{
		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_TweenFrames", &m_TweenDesc, sizeof(TWEEN_DESC))))
			return E_FAIL;
	}

	const _uint		iNumMeshes = m_pRenderModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pRenderModelCom->Render(m_pShaderCom, i, 3)))
			return E_FAIL;
	}
	


	return S_OK;
}

//
//HRESULT CCounter_FX::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
//{
//
//	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
//		return E_FAIL;
//
//
//	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
//		return E_FAIL;
//
//	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
//		return E_FAIL;
//
//	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
//		return E_FAIL;
//
//	if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
//		return E_FAIL;
//
//	if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
//	{
//		if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
//			return E_FAIL;
//	}
//	
//
//	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
//		return E_FAIL;
//
//	// Bloom -----------------
//	if (FAILED(pInstancingShader->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
//		return E_FAIL;
//	// ----------------- Bloom
//
//	// RimLight -----------------
//	if (FAILED(pInstancingShader->Bind_RawValue("g_vRimColor", &m_vRimColor, sizeof(_float4))))
//		return E_FAIL;
//	// ----------------- RimLight
//
//	if (FAILED(pInstancingShader->Bind_RawValue("g_fMotionTrailAlpha", &m_fAlpha, sizeof(_float))))
//		return E_FAIL;
//
//	_uint iNumMeshes = m_pRenderModelCom->Get_NumMeshes();
//	_uint iPassIndex = 0;
//
//	for (_uint i = 0; i < iNumMeshes; ++i)
//	{
//		if (FAILED(m_pRenderModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 3)))
//			return E_FAIL;
//	}
//
//	return S_OK;
//}




HRESULT CCounter_FX::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}



CCounter_FX* CCounter_FX::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCounter_FX* pInstance = new CCounter_FX(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CCounter_FX");
		Safe_Release<CCounter_FX*>(pInstance);
	}

	return pInstance;
}

CGameObject* CCounter_FX::Clone(void* pArg)
{
	CCounter_FX* pInstance = new CCounter_FX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CCounter_FX");
		Safe_Release<CCounter_FX*>(pInstance);
	}

	return pInstance;
}

void CCounter_FX::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRenderModelCom);
}

#include "stdafx.h"
#include "..\Public\ColliderWall.h"
#include "GameInstance.h"


CColliderWall::CColliderWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CStaticObject(pDevice, pContext, strObjectTag, eType)
{
}

CColliderWall::CColliderWall(const CStaticObject& rhs)
	: CStaticObject(rhs)
{
}

HRESULT CColliderWall::Initialize_Prototype()
{
	// Rect


	return S_OK;
}

HRESULT CColliderWall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CColliderWall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CColliderWall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 5.0f))
	//{
		// Shadow 필요하면 ShadowRender 추가?
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	//}
}

HRESULT CColliderWall::Render()
{
	if (true == m_bEnable)
		return S_OK;

	if (FAILED(m_pNonAnimShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pNonAnimShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pNonAnimShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pNonAnimShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;

	return S_OK;
}

HRESULT CColliderWall::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (true == m_bEnable)
		return S_OK;

	if (nullptr == pInstancingShader)
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if(FAILED(m_pNonAnimShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CColliderWall::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CColliderWall::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CColliderWall::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CColliderWall::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPos"),
		TEXT("Com_NonAnim_Shader"), reinterpret_cast<CComponent**>(&m_pNonAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CColliderWall* CColliderWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CColliderWall* pInstance = new CColliderWall(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CColliderWall");
		Safe_Release<CColliderWall*>(pInstance);
	}

	return pInstance;
}

CGameObject* CColliderWall::Clone(void* pArg)
{
	CColliderWall* pInstance = new CColliderWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CColliderWall");
		Safe_Release<CColliderWall*>(pInstance);
	}

	return pInstance;
}

void CColliderWall::Free()
{
	__super::Free();

	Safe_Release<CVIBuffer_Rect*>(m_pVIBufferCom);
}

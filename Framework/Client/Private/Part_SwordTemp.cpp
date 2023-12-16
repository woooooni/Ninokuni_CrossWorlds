#include "stdafx.h"
#include "..\Public\Part_SwordTemp.h"

#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Collision_Manager.h"
#include "Monster.h"
#include "PipeLine.h"
#include "Particle_Manager.h"

CPart_SwordTemp::CPart_SwordTemp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CPart(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_WEAPON)
{

}

CPart_SwordTemp::CPart_SwordTemp(const CPart_SwordTemp& rhs)
	: CPart(rhs)
{

}

HRESULT CPart_SwordTemp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPart_SwordTemp::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CPart_SwordTemp::Tick(_float fTimeDelta)
{
	
}

void CPart_SwordTemp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CPart_SwordTemp::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint		iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

void CPart_SwordTemp::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CPart_SwordTemp::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CPart_SwordTemp::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CPart_SwordTemp::Ready_Components()
{

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_TempSword"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPart_SwordTemp::Ready_Colliders()
{
	return S_OK;
}

HRESULT CPart_SwordTemp::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	//m_pModelCom->Get_PivotMatrix();

	Matrix matWorld = m_matSocketWorld * m_pTransformCom->Get_WorldMatrix();

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &matWorld.Transpose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}


CPart_SwordTemp* CPart_SwordTemp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CPart_SwordTemp* pInstance = new CPart_SwordTemp(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPart_SwordTemp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPart_SwordTemp::Clone(void* pArg)
{
	CPart_SwordTemp* pInstance = new CPart_SwordTemp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPart_SwordTemp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPart_SwordTemp::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Door_Enter_FX.h"

CDoor_Enter_FX::CDoor_Enter_FX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Door_Enter_FX", OBJ_TYPE::OBJ_EFFECT)
{
}

CDoor_Enter_FX::CDoor_Enter_FX(const CDoor_Enter_FX& rhs)
	: CGameObject(rhs)
{
}


HRESULT CDoor_Enter_FX::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor_Enter_FX::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);
	m_pTransformCom->Set_Scale(_float3(0.03f, 0.03f, 0.03f));

	m_fWaitTime = 0.7f;
	return S_OK;
}

void CDoor_Enter_FX::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (false == m_bPlayAnimation)
	{
		if(fTimeDelta <= 0.07f)
			m_fAccWait += fTimeDelta;

		if (m_fAccWait >= m_fWaitTime)
		{
			m_bPlayAnimation = true;
			m_fAccWait = 0.f;
		}
	}
	


	if (false == m_bReserveDead && true == m_pModelCom->Is_Finish())
		Reserve_Dead(true);


	if (true == m_bReserveDead)
	{
		m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
		if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_Dead(true);
			return;
		}
	}

}

void CDoor_Enter_FX::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(true == m_bPlayAnimation)
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CDoor_Enter_FX::Render()
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	// Bloom -----------------
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;
	// ----------------- Bloom

	// RimLight -----------------
	_float4 vRimLightColor = _float4(0.f, 0.f, 0.f, 0.f);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimLightColor, sizeof(_float4))))
		return E_FAIL;
	// ----------------- RimLight

	// Dissolve -----------------
	if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	// ----------------- Dissolve

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (m_bReserveDead)
			iPassIndex = 2;
		else if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}



HRESULT CDoor_Enter_FX::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DoorEnter"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_DissolveTexture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"), TEXT("Com_DissolveTexture"), (CComponent**)&m_pDissolveTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor_Enter_FX::Ready_Collider()
{

	return S_OK;
}


CDoor_Enter_FX* CDoor_Enter_FX::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDoor_Enter_FX* pInstance = new CDoor_Enter_FX(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDoor_Enter_FX");
		Safe_Release<CDoor_Enter_FX*>(pInstance);
	}

	return pInstance;
}

CGameObject* CDoor_Enter_FX::Clone(void* pArg)
{
	CDoor_Enter_FX* pInstance = new CDoor_Enter_FX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDoor_Enter_FX");
		Safe_Release<CDoor_Enter_FX*>(pInstance);
	}

	return pInstance;
}

void CDoor_Enter_FX::Free()
{
	__super::Free();
	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	
}

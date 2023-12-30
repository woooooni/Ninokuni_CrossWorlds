#include "stdafx.h"
#include "..\Public\SkyDome.h"
#include "GameInstance.h"


CSkyDome::CSkyDome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CSkyDome::CSkyDome(const CSkyDome& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyDome::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyDome::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSkyDome::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn(Vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.005f, fTimeDelta);
}

void CSkyDome::LateTick(_float fTimeDelta)
{
	Vec4 vCamePos = GI->Get_CamPosition();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamePos);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this);
}

HRESULT CSkyDome::Render()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("viewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("projectionMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	//
	if (FAILED(m_pShaderCom->Bind_RawValue("apexColor", &m_vApexColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("centerColor", &m_vCenterColor, sizeof(Vec4))))
		return E_FAIL;
	//

	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "SkydomeTexture")))
	//	return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "SkydomeTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSkyDome::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Skydome"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sky_dome"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sky_DomeTex"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CSkyDome* CSkyDome::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CSkyDome* pInstance = new CSkyDome(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyDome");
		Safe_Release<CSkyDome*>(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyDome::Clone(void* pArg)
{
	CSkyDome* pInstance = new CSkyDome(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkyDome");
		Safe_Release<CSkyDome*>(pInstance);
	}

	return pInstance;
}

void CSkyDome::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CTexture*>(m_pTextureCom);
	Safe_Release<CModel*>(m_pModelCom);

}

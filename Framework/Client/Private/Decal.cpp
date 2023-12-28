#include "stdafx.h"
#include "Decal.h"

#include "GameInstance.h"

#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"

CDecal::CDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
	, m_isCloned(false)
{
}

CDecal::CDecal(const CDecal& rhs)
	: CGameObject(rhs)
	, m_isCloned(true)
	, m_tDecalDesc(rhs.m_tDecalDesc)
{
}

HRESULT CDecal::Initialize_Prototype(const DECAL_DESC* pDecalDesc, const wstring& strDecalFilePath)
{
	if (pDecalDesc != nullptr)
		m_tDecalDesc = *pDecalDesc;

 	return S_OK;
}

HRESULT CDecal::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(5.f, 1.f, 5.f));

	return S_OK;
}

void CDecal::Tick(_float fTimeDelta)
{
	if (Is_Dead() == true)
		return;


}

void CDecal::LateTick(_float fTimeDelta)
{
	if (Is_Dead() == true)
		return;

	__super::LateTick(fTimeDelta);

	//if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CDecal::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(m_tDecalDesc.iShaderPass);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CDecal::Bind_ShaderResource()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorldInv", &m_pTransformCom->Get_WorldMatrix_Float4x4_Inverse())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matViewInv", &GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProjInv", &GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(GI->Bind_SRV(m_pShaderCom, TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;

	// 이펙트 정보
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_EffectDesc", m_pVIBufferCom->Get_ParticleShaderInfo().data(), sizeof(CVIBuffer_Particle::PARTICLE_SHADER_DESC) * m_pVIBufferCom->Get_ParticleShaderInfo().size())))
	//	return E_FAIL;

	// 텍스처
	if (m_pDiffuseTextureCom != nullptr)
	{
		if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Diffuse2DTexture", m_tDecalDesc.iTextureIndexDiffuse)))
			return E_FAIL;
	}
	if (m_pAlphaTextureCom != nullptr)
	{
		if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Alpha2DTexture", m_tDecalDesc.iTextureIndexAlpha)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha_Discard", &m_tDecalDesc.fAlpha_Discard, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlack_Discard", &m_tDecalDesc.fBlack_Discard, sizeof(_float3))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDecal::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal"),
		TEXT("Com_Diffuse_Texture"), (CComponent**)&m_pDiffuseTextureCom)))
		return E_FAIL;
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal"),
	//	TEXT("Com_Alpha_Texture"), (CComponent**)&m_pAlphaTextureCom)))
	//	return E_FAIL;

	return S_OK;
}

CDecal* CDecal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const DECAL_DESC* pDecalDesc, const wstring& strDecalFilePath)
{
	CDecal* pInstance = new CDecal(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype(pDecalDesc, strDecalFilePath)))
	{
		MSG_BOX("Failed to Created : CDecal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDecal::Clone(void* pArg)
{
	CDecal* pInstance = new CDecal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDecal");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDecal::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
}
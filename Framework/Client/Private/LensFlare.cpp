#include "stdafx.h"
#include "..\Default\LensFlare.h"
#include "GameInstance.h"

CLensFlare::CLensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CLensFlare::CLensFlare(const CLensFlare& rhs)
	: CGameObject(rhs)
{

}

HRESULT CLensFlare::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLensFlare::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CLensFlare::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CLensFlare::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_LENSFLARE, this);
}

HRESULT CLensFlare::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render(7);

	return S_OK;
}

HRESULT CLensFlare::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LensFlare"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LensFlare_Texture"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLensFlare::Bind_ShaderResources()
{
	const LIGHTDESC* pLightDesc = GI->Get_LightDesc(0);
	Vec4 light_ss{};
	{
		Vec4 vCamPos = GI->Get_CamPosition();
		Vec4 vLightDir = pLightDesc->vDirection;
		Vec4 vLightPos = vCamPos - vLightDir;

		Vec4 light_pos = Vec4::Transform(vLightPos, GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ));
		light_ss.x = 0.5f * light_pos.x / light_pos.w + 0.5f;
		light_ss.y = -0.5f * light_pos.y / light_pos.w + 0.5f;
		light_ss.z = light_pos.z / light_pos.w;
		light_ss.w = 1.0f;
	}

	// Screen Space Position
	m_LensDesc.vColor = pLightDesc->vDiffuse;
	m_LensDesc.vDirection = pLightDesc->vDirection;
	m_LensDesc.vScreen_space_position = light_ss;

	if (FAILED(m_pShaderCom->Bind_RawValue("current_light", &m_LensDesc, sizeof(CLensFlare::LENSFLARE_DESC))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens0", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens1", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens2", 2)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens3", 3)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens4", 4)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens5", 5)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "lens6", 6)))
		return E_FAIL;
	// DepthTexture
	if (FAILED(GI->Bind_SRV(m_pShaderCom, TEXT("Target_Depth"), "DepthTexture")))
		return E_FAIL;

	
	return S_OK;
}

CLensFlare* CLensFlare::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CLensFlare* pInstance = new CLensFlare(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLensFlare");
		Safe_Release<CLensFlare*>(pInstance);
	}

	return pInstance;
}

CGameObject* CLensFlare::Clone(void* pArg)
{
	CLensFlare* pInstance = new CLensFlare(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLensFlare");
		Safe_Release<CLensFlare*>(pInstance);
	}

	return pInstance;
}

void CLensFlare::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CVIBuffer_LensFlare*>(m_pVIBufferCom);
	Safe_Release<CTexture*>(m_pTextureCom);
}

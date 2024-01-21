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

	Vec4 light_ss{};
	{	
		const LIGHTDESC* pLightDesc = GI->Get_LightDesc(0);
		Vec4 vCamPos = GI->Get_CamPosition();
		Vec4 vLightDir = pLightDesc->vDirection;
		Vec4 vLightPos = vCamPos - vLightDir;

		Matrix view = GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
		Matrix proj = GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
		Matrix viewProj = view * proj;

		Vec4 ProjPos;
		ProjPos = Vec4::Transform(vLightPos, viewProj);

		ProjPos.x = (ProjPos.x + 1.0f) * 0.5f;
		ProjPos.y = (ProjPos.y - 1.0f) * -0.5f;

		light_ss.x = ProjPos.x;
		light_ss.y = ProjPos.y;
		light_ss.z = ProjPos.z / ProjPos.w;
		light_ss.w = 1.0f;
	}
	Vec4 vScreenSunPos = light_ss;
	if (vScreenSunPos.x > 1.05f || vScreenSunPos.x < -0.05f
		|| vScreenSunPos.y > 1.05f || vScreenSunPos.y < -0.05f
		|| vScreenSunPos.z > 1.f || vScreenSunPos.z < 0.f)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_LENSFLARE, this);
}

HRESULT CLensFlare::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	if(FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if(FAILED(m_pVIBufferCom->Render(7)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLensFlare::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LensFlare"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_LensFlare"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
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

		Matrix view = GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
		Matrix proj = GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
		Matrix viewProj = view * proj;

		Vec4 ProjPos;
		ProjPos = Vec4::Transform(vLightPos, viewProj);

		ProjPos.x = (ProjPos.x + 1.0f) * 0.5f;
		ProjPos.y = (ProjPos.y - 1.0f) * -0.5f;
		
		light_ss.x = ProjPos.x;
		light_ss.y = ProjPos.y;
		light_ss.z = ProjPos.z / ProjPos.w;
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
	if (FAILED(GI->Bind_SRV(m_pShaderCom, TEXT("Target_Depth"), "depth_texture")))
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

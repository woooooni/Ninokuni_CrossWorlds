#include "stdafx.h"
#include "GameInstance.h"
#include "Trail.h"


USING(Client)
CTrail::CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_TRAIL)
{
}

CTrail::CTrail(const CTrail& rhs)
	: CGameObject(rhs)
{	

}


HRESULT CTrail::Initialize_Prototype(const TRAIL_DESC& TrailDesc)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	m_TrailDesc = TrailDesc;

	return S_OK;
}

HRESULT CTrail::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTrail::Tick(_float fTimeDelta)
{
	m_TrailDesc.vUVAcc.x += m_TrailDesc.vUV_FlowSpeed.x * fTimeDelta;
	m_TrailDesc.vUVAcc.y += m_TrailDesc.vUV_FlowSpeed.y * fTimeDelta;


	m_pVIBufferCom->Update_TrailBuffer(fTimeDelta, XMLoadFloat4x4(&m_TransformMatrix));
		
}

void CTrail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	/*m_TrailDesc.bTrail = true;
	m_TrailDesc.vDiffuseColor = { 1.f, 0.3f, 0.f, 0.f };
	m_TrailDesc.vBlackDiscard = { 0.5f, 0.5f, 0.5f };
	m_TrailDesc.fBlurPower = 0.f;
	m_TrailDesc.vDistortion = { 0.5f, 0.5f };

	m_iDiffuseTextureIndex = 7;
	m_iAlphaTextureIndex = 7;
	SetUp_Position(XMVectorSet(0.f, 0.f, 0.5f, 1.f), XMVectorSet(0.f, 0.f, 1.5f, 1.f));
	m_pVIBufferCom->Set_VtxCount(44);
	*/
	

	if(true == m_TrailDesc.bTrail)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
	
}

HRESULT CTrail::Render()
{
	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMMatrixIdentity()));

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_TrailDesc.vDiffuseColor, sizeof(_float4))))
		return E_FAIL;

	_float fAlpha = m_TrailDesc.vDiffuseColor.w;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha_Discard", &m_TrailDesc.fAlphaDiscard, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlack_Discard", &m_TrailDesc.vBlackDiscard, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion", &m_TrailDesc.vDistortion, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vUVFlow", &m_TrailDesc.vUVFlow, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vUVIndex", &m_TrailDesc.vUVIndex, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vMaxUVCount", &m_TrailDesc.vMaxUVCount, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_iUVLoop", &m_TrailDesc.bUV_Cut, sizeof(_int))))
		return E_FAIL;

	

	if (FAILED(m_pDistortionTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DistortionTexture", m_iDistortionTextureIndex)))
		return E_FAIL;


	_uint iPassIndex = 0;
	{

		if (-1 < m_iDiffuseTextureIndex)
		{
			if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iDiffuseTextureIndex)))
				return E_FAIL;
		}


		if (-1 < m_iAlphaTextureIndex)
		{
			if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(m_pShaderCom, "g_AlphaTexture", m_iAlphaTextureIndex)))
				return E_FAIL;
		}


		// 둘다 없다면.
		if (-1 == m_iDiffuseTextureIndex && -1 == m_iAlphaTextureIndex)
			iPassIndex = 0;

		// 디퓨즈 텍스쳐만 있다면.
		if (-1 != m_iDiffuseTextureIndex && -1 == m_iAlphaTextureIndex)
			iPassIndex = 1;

		// 알파 텍스쳐만 있다면.
		if (-1 == m_iDiffuseTextureIndex && -1 != m_iAlphaTextureIndex)
			iPassIndex = 2;

		if (-1 != m_iDiffuseTextureIndex && -1 != m_iAlphaTextureIndex)
			iPassIndex = 3; 
	}

	if (FAILED(m_pShaderCom->Begin(iPassIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CTrail::Set_DiffuseTexture_Index(const wstring& strDiffuseTextureName)
{
	m_iDiffuseTextureIndex = m_pDiffuseTextureCom->Find_Index(strDiffuseTextureName);
}

void CTrail::Set_AlphaTexture_Index(const wstring& strAlphaTextureName)
{
	m_iAlphaTextureIndex = m_pAlphaTextureCom->Find_Index(strAlphaTextureName);
}

void CTrail::Set_DistortionTexture_Index(const wstring& strDistortionTextureName)
{
	_int  i = m_pDistortionTextureCom->Find_Index(strDistortionTextureName);
	m_iDistortionTextureIndex = i == -1 ? 0 : i;
}

HRESULT CTrail::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Trail"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &m_TrailDesc)))
		return E_FAIL;

	/* For.Com_Diffuse_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Diffuse"), TEXT("Com_Diffuse_Texture"), (CComponent**)&m_pDiffuseTextureCom)))
		return E_FAIL;

	/* For.Com_Alpha_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Alpha"), TEXT("Com_Alpha_Texture"), (CComponent**)&m_pAlphaTextureCom)))
		return E_FAIL;

	/* For.Com_Distortion_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Distiortion"), TEXT("Com_Distortion_Texture"), (CComponent**)&m_pDistortionTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CTrail::SetUp_Position(_vector vHighPosition, _vector vLowPosition)
{
	m_pVIBufferCom->SetUp_Position(vHighPosition, vLowPosition);
}

void CTrail::Start_Trail(_matrix TransformMatrix)
{
	m_TrailDesc.bTrail = true;

	m_TrailDesc.vUVAcc.x = 0.f;
	m_TrailDesc.vUVAcc.y = 0.f;

	m_pVIBufferCom->Start_Trail(TransformMatrix);
}

void CTrail::Stop_Trail()
{
	m_TrailDesc.bTrail = false;
	m_pVIBufferCom->Stop_Trail();
}


CTrail* CTrail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const TRAIL_DESC& TrailDesc)
{
	CTrail* pInstance = new CTrail(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype(TrailDesc)))
	{
		MSG_BOX("Create Failed : CTrail");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CTrail::Clone(void* pArg)
{
	CTrail* pInstance = new CTrail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
	Safe_Release(m_pDistortionTextureCom);
	Safe_Release(m_pVIBufferCom);

}

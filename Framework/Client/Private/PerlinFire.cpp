#include "stdafx.h"
#include "..\Public\PerlinFire.h"
#include "GameInstance.h"
#include "Quest_Manager.h"

CPerlinFire::CPerlinFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CPerlinFire::CPerlinFire(const CPerlinFire& rhs)
	: CGameObject(rhs)
{

}

HRESULT CPerlinFire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPerlinFire::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CPerlinFire::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime += fTimeDelta;
	if (m_fTime > 1000.0f)
		m_fTime = 0.0f;
}

void CPerlinFire::LateTick(_float fTimeDelta)
{
	Matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	_matrix Temp = XMLoadFloat4x4(&WorldMatrix);
	_vector vPosition = Temp.r[CTransform::STATE_POSITION];
	_vector vCamPosition = XMLoadFloat4(&GI->Get_CamPosition());

	_float fLookScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_LOOK]));
	_vector vLook = XMVectorSetW(XMVector3Normalize(vPosition - vCamPosition), 0.f) * fLookScale;

	_float fRightScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_RIGHT]));
	_vector vRight = XMVectorSetW(XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)), 0.f) * fRightScale;

	_float fUpScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_UP]));
	_vector vUp = XMVectorSetW(XMVector3Normalize(XMVector3Cross(vLook, vRight)), 0.f) * fUpScale;

	Temp.r[CTransform::STATE_RIGHT] = vRight;
	Temp.r[CTransform::STATE_UP] = vUp;
	Temp.r[CTransform::STATE_LOOK] = vLook;

	XMStoreFloat4x4(&WorldMatrix, Temp);
	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_ALPHABLEND, this);
}

HRESULT CPerlinFire::Render()
{
	Matrix world, view, Projection;
	world = m_pTransformCom->Get_WorldMatrix();
	view = GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	Projection = GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);


	Matrix WorldView, WorldViewProj;
	WorldView = world * view;
	WorldViewProj = WorldView * Projection;

	if (FAILED(m_pShaderCom->Bind_Matrix("WorldViewProj", &WorldViewProj)))
		return E_FAIL;
	{
		m_vScrollSpeeds = Vec3(1.3f, 2.1f, 2.3f);
		m_vScales = Vec3(1.0f, 2.0f, 3.0f);

		m_vDistortion1 = Vec2(0.1f, 0.2f);
		m_vDistortion2 = Vec2(0.1f, 0.3f);
		m_vDistortion3 = Vec2(0.1f, 0.1f);

		m_fDistortionScale = 0.2f;
		m_fDistortionBias = 0.1f;
	}

	// Perlin Fire
	if (FAILED(m_pTextureCom[TEX_TYPE::DIFFUSE_TEX]->Bind_ShaderResource(m_pShaderCom, "fireTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::NOISE_TEX]->Bind_ShaderResource(m_pShaderCom, "noiseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::ALPHA_TEX]->Bind_ShaderResource(m_pShaderCom, "alphaTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("frameTime", &m_fTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("scrollSpeeds", &m_vScrollSpeeds, sizeof(Vec3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("scales", &m_vScales, sizeof(Vec3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("distortion1", &m_vDistortion1, sizeof(Vec2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("distortion2", &m_vDistortion2, sizeof(Vec2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("distortion3", &m_vDistortion3, sizeof(Vec2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("distortionScale", &m_fDistortionScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("distortionBias", &m_fDistortionBias, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Begin(8)))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPerlinFire::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{

	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	{
		m_vScrollSpeeds = Vec3(1.3f, 2.1f, 2.3f);
		m_vScales = Vec3(1.0f, 2.0f, 3.0f);

		m_vDistortion1 = Vec2(0.1f, 0.2f);
		m_vDistortion2 = Vec2(0.1f, 0.3f);
		m_vDistortion3 = Vec2(0.1f, 0.1f);

		m_fDistortionScale = 0.2f;
		m_fDistortionBias = 0.1f;
	}

	// Perlin Fire
	if (FAILED(m_pTextureCom[TEX_TYPE::DIFFUSE_TEX]->Bind_ShaderResource(pInstancingShader, "fireTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::NOISE_TEX]->Bind_ShaderResource(pInstancingShader, "noiseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::ALPHA_TEX]->Bind_ShaderResource(pInstancingShader, "alphaTexture", 0)))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("frameTime", &m_fTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("scrollSpeeds", &m_vScrollSpeeds, sizeof(Vec3))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("scales", &m_vScales, sizeof(Vec3))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("distortion1", &m_vDistortion1, sizeof(Vec2))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("distortion2", &m_vDistortion2, sizeof(Vec2))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("distortion3", &m_vDistortion3, sizeof(Vec2))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("distortionScale", &m_fDistortionScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("distortionBias", &m_fDistortionBias, sizeof(_float))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Begin(1)))
		return E_FAIL;
	if (FAILED(pInstancingBuffer->Render(WorldMatrices, m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPerlinFire::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FireDiffuse"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::DIFFUSE_TEX]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FireNoise"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::NOISE_TEX]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FireAlpha"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::ALPHA_TEX]))))
		return E_FAIL;

	return S_OK;
}

CPerlinFire* CPerlinFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CPerlinFire* pInstance = new CPerlinFire(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPerlinFire");
		Safe_Release<CPerlinFire*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPerlinFire::Clone(void* pArg)
{
	CPerlinFire* pInstance = new CPerlinFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPerlinFire");
		Safe_Release<CPerlinFire*>(pInstance);
	}

	return pInstance;
}

void CPerlinFire::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIBufferCom);

	for (_uint i = 0; i < TEX_TYPE::TEX_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);
}

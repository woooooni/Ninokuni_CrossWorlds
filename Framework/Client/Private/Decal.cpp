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

HRESULT CDecal::Initialize_Prototype(const DECAL_DESC* pDecalDesc)
{
	if (pDecalDesc != nullptr)
		m_tDecalDesc = *pDecalDesc;

 	return S_OK;
}

HRESULT CDecal::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	Restart_Decal();

	return S_OK;
}

void CDecal::Tick(_float fTimeDelta)
{
	if (Is_Dead() == true)
		return;

	if (m_bDecalDelete)
	{
		m_fAccLifeTime += fTimeDelta;
		if (m_bDecalDie || m_fAccLifeTime >= m_tDecalDesc.fLifeTime)
		{
			m_bDecalDie = true;
			if (GI->Get_CurrentLevel() != LEVEL_TOOL)
				Set_Dead(true);
		}
	}

	Tick_Alpha(fTimeDelta);
}

void CDecal::LateTick(_float fTimeDelta)
{
	if (Is_Dead() == true || m_bDecalDie)
		return;

	__super::LateTick(fTimeDelta);

	if (m_pOwnerObject != nullptr)
	{
		if (m_pOwnerObject->Is_Dead())
		{
			Set_Dead(true);
			return;
		}

		CTransform* pTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
		if (pTransform != nullptr)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position() + ::XMLoadFloat4(&m_vOffsetPos));
	}

	if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 10.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DECAL, this);
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

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrixInv", &m_pTransformCom->Get_WorldMatrix_Float4x4_Inverse())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", &GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", &GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(GI->Bind_SRV(m_pShaderCom, TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;

	// 텍스처
	if (m_pDiffuseTextureCom != nullptr)
	{
		if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Diffuse2DTexture", m_tDecalDesc.iTextureIndexDiffuse)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha_Discard", &m_tDecalDesc.fAlpha_Discard, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlack_Discard", &m_tDecalDesc.fBlack_Discard, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fColor_Add_01_Alpha", &m_tDecalDesc.fColorAdd_01_Alpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fColor_Add_01", &m_tDecalDesc.fColorAdd_01, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fColor_Add_02", &m_tDecalDesc.fColorAdd_02, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fColor_Alpha", &m_tDecalDesc.fAlphaRemove, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBloom_Power", &m_tDecalDesc.fBloomPower, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlur_Power", &m_tDecalDesc.fBlurPower, sizeof(_float))))
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal"), TEXT("Com_Texture"), (CComponent**)&m_pDiffuseTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CDecal::Set_DecalDesc(const DECAL_DESC& tDesc)
{
	m_tDecalDesc = tDesc;

	if (m_pDiffuseTextureCom != nullptr && m_tDecalDesc.iTextureIndexDiffuse >= m_pDiffuseTextureCom->Get_TextureCount())
		m_tDecalDesc.iTextureIndexDiffuse = m_pDiffuseTextureCom->Get_TextureCount() - 1;

	Restart_Decal();

	m_bDecalDie = false;
}

void CDecal::Restart_Decal()
{
	if (m_pTransformCom == nullptr)
		return;

	m_bDecalDie    = false;
	m_fAccLifeTime = 0.f;

	m_pTransformCom->Set_Scale(m_tDecalDesc.fScale);

	if (m_tDecalDesc.bAlphaCreate)
	{
		m_tDecalDesc.fAlphaRemove = 1.f;
		m_bAlphaCreateSucc = false;
	}
	else if (m_tDecalDesc.bAlphaDelete)
		m_bAlphaCreateSucc = true;
}

void CDecal::Tick_Alpha(_float fTimeDelta)
{
	// 생성 이벤트 1 -> 0
	if (m_tDecalDesc.bAlphaCreate && !m_bAlphaCreateSucc && m_tDecalDesc.fAlphaRemove > 0.f)
	{
		m_tDecalDesc.fAlphaRemove -= fTimeDelta * m_tDecalDesc.fAlphaSpeed;
		if (m_tDecalDesc.fAlphaRemove <= 0.f)
		{
			m_tDecalDesc.fAlphaRemove = 0.f;
			m_bAlphaCreateSucc = true;
		}
	}

	// 삭제 이벤트 0 -> 1
	else if (m_tDecalDesc.bAlphaDelete && m_bAlphaCreateSucc && m_fAccLifeTime > m_tDecalDesc.fLifeTime - 2.f)
	{
		m_tDecalDesc.fAlphaRemove += fTimeDelta * m_tDecalDesc.fAlphaSpeed;
		if (m_tDecalDesc.fAlphaRemove >= 1.f)
			m_tDecalDesc.fAlphaRemove = 1.f;
	}
}

CDecal* CDecal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const DECAL_DESC* pDecalDesc)
{
	CDecal* pInstance = new CDecal(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype(pDecalDesc)))
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
}
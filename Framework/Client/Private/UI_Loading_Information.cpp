#include "stdafx.h"
#include "UI_Loading_Information.h"
#include "GameInstance.h"

CUI_Loading_Logo::CUI_Loading_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Loading_Information")
{
}

CUI_Loading_Logo::CUI_Loading_Logo(const CUI_Loading_Logo& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading_Logo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Logo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 1.f;

	return S_OK;
}

void CUI_Loading_Logo::Tick(_float fTimeDelta)
{
	if (m_iTextureIndex < 0 || 2 < m_iTextureIndex)
		return;

	__super::Tick(fTimeDelta);
}

void CUI_Loading_Logo::LateTick(_float fTimeDelta)
{
	if (m_iTextureIndex < 0 || 2 < m_iTextureIndex)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Loading_Logo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading_Logo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Information"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Logo::Ready_State()
{
	m_tInfo.fCX = 486.f * 0.9f;
	m_tInfo.fCY = 236.f * 0.9f;
	m_tInfo.fX = (g_iWinSizeX * 0.5f) + 475.f; // + 60
	m_tInfo.fY = g_iWinSizeY * 0.5f + 200.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Loading_Logo::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Loading_Logo * CUI_Loading_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Loading_Logo*	pInstance = new CUI_Loading_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Loading_Logo::Clone(void* pArg)
{
	CUI_Loading_Logo*	pInstance = new CUI_Loading_Logo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Logo::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

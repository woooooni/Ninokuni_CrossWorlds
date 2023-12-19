#include "stdafx.h"
#include "UI_Loading_Imajinn.h"
#include "GameInstance.h"

CUI_Loading_Imajinn::CUI_Loading_Imajinn(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Loading_Imajinn")
{
}

CUI_Loading_Imajinn::CUI_Loading_Imajinn(const CUI_Loading_Imajinn& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading_Imajinn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Imajinn::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.8f;

	return S_OK;
}

void CUI_Loading_Imajinn::Tick(_float fTimeDelta)
{
	m_fFrame += 7.f * fTimeDelta;

	if (m_fFrame > 9.f)
		m_fFrame = 0.f;

	__super::Tick(fTimeDelta);
}

void CUI_Loading_Imajinn::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Loading_Imajinn::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading_Imajinn::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Imajinn"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Imajinn::Ready_State()
{
	m_tInfo.fCX = 246.f * 0.4f;
	m_tInfo.fCY = 246.f * 0.5f;
	m_tInfo.fX = (g_iWinSizeX * 0.5f) + 685.f;
	m_tInfo.fY = g_iWinSizeY * 0.5f + 280.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Loading_Imajinn::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Loading_Imajinn * CUI_Loading_Imajinn::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Loading_Imajinn*	pInstance = new CUI_Loading_Imajinn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_Imajinn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Loading_Imajinn::Clone(void* pArg)
{
	CUI_Loading_Imajinn*	pInstance = new CUI_Loading_Imajinn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_Imajinn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Imajinn::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

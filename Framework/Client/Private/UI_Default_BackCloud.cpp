#include "stdafx.h"
#include "UI_Default_BackCloud.h"
#include "GameInstance.h"

CUI_Default_BackCloud::CUI_Default_BackCloud(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Default_BackCloud")
{
}

CUI_Default_BackCloud::CUI_Default_BackCloud(const CUI_Default_BackCloud& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Default_BackCloud::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Default_BackCloud::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.4f;
	m_bActive = true;

	return S_OK;
}

void CUI_Default_BackCloud::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		
		__super::Tick(fTimeDelta);
	}
}

void CUI_Default_BackCloud::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_fTime += fTimeDelta;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		//__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Default_BackCloud::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(3);

		m_pVIBufferCom->Render();

		//__super::Render();
	}

	return S_OK;
}

HRESULT CUI_Default_BackCloud::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DefaultBackground_Cloud"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Default_BackCloud::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Default_BackCloud::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Default_BackCloud * CUI_Default_BackCloud::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Default_BackCloud*	pInstance = new CUI_Default_BackCloud(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Default_BackCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Default_BackCloud::Clone(void* pArg)
{
	CUI_Default_BackCloud*	pInstance = new CUI_Default_BackCloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Default_BackCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Default_BackCloud::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

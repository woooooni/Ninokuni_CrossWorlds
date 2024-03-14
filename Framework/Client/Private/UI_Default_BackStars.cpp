#include "stdafx.h"
#include "UI_Default_BackStars.h"
#include "GameInstance.h"

CUI_Default_BackStars::CUI_Default_BackStars(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Default_BackStars")
{
}

CUI_Default_BackStars::CUI_Default_BackStars(const CUI_Default_BackStars& rhs)
	: CUI(rhs)
{
}

void CUI_Default_BackStars::Set_Active(_bool bActive)
{
	if (bActive)
	{

	}
	else
	{
		m_iTextureIndex = 0;
		m_fAlpha = 0.1f;
		m_bActive = false;
		m_bReverse = true;
	}

	m_bActive = bActive;
}

HRESULT CUI_Default_BackStars::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Default_BackStars::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.1f;

	m_bActive = false;
	m_bReverse = true;

	return S_OK;
}

void CUI_Default_BackStars::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_bReverse)
		{
			m_fAlpha -= fTimeDelta * 0.5f;

			if (0.1f > m_fAlpha)
			{
				m_bReverse = true;
				m_fAlpha = 0.1f;

				if (0 <= m_iTextureIndex)
					m_iTextureIndex++;
			}
		}
		else
		{
			m_fAlpha += fTimeDelta * 0.5f;

			if (1.f < m_fAlpha)
			{
				m_bReverse = false;
				m_fAlpha = 1.f;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Default_BackStars::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (2 < m_iTextureIndex)
			m_iTextureIndex = 0;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Default_BackStars::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Default_BackStars::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_DefaultBackground_StarEffect"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Default_BackStars::Ready_State()
{
	m_tInfo.fCX = g_iWinSizeX;
	m_tInfo.fCY = g_iWinSizeY;

	m_tInfo.fX = g_iWinSizeX * 0.5f;
	m_tInfo.fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Default_BackStars::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_Default_BackStars * CUI_Default_BackStars::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Default_BackStars*	pInstance = new CUI_Default_BackStars(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Default_BackStars");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Default_BackStars::Clone(void* pArg)
{
	CUI_Default_BackStars*	pInstance = new CUI_Default_BackStars(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Default_BackStars");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Default_BackStars::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

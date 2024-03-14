#include "stdafx.h"
#include "UI_Grandprix_Vignette.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Grandprix_Vignette::CUI_Grandprix_Vignette(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIX_VIGNETTE eType)
	: CUI(pDevice, pContext, L"UI_Grandprix_Vignette")
	, m_eType(eType)
{
}

CUI_Grandprix_Vignette::CUI_Grandprix_Vignette(const CUI_Grandprix_Vignette& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Grandprix_Vignette::Set_Active(_bool bActive)
{
	if (VIGNETTE_DAMAGED == m_eType)
	{
		if (true == bActive)
		{
			m_fAlpha = 0.f;
			m_bSetAlpha = false;
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_Grandprix_Vignette::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_Vignette::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Grandprix_Vignette::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (VIGNETTE_RUSH == m_eType)
		{
			m_fTimeAcc += fTimeDelta;

			if (0.1f < m_fTimeAcc)
			{
				if (m_iTextureIndex == 6)
					m_iTextureIndex = 0;
				else
					m_iTextureIndex++;

				m_fTimeAcc = 0.f;
			}
		}
		else if (VIGNETTE_DAMAGED == m_eType)
		{
			if (true == m_bSetAlpha)
			{
				m_fAlpha -= fTimeDelta * 1.2f;

				if (m_fAlpha <= 0.f)
				{
					m_fAlpha = 0.f;
					Set_Active(false);
				}
			}
			else
			{
				m_fAlpha += fTimeDelta * 1.2f;

				if (m_fAlpha >= 1.f)
				{
					m_fAlpha = 1.f;
					m_bSetAlpha = true;
				}
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_Vignette::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_EFFECT_BLEND, this);
	}
}

HRESULT CUI_Grandprix_Vignette::Render()
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

HRESULT CUI_Grandprix_Vignette::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case VIGNETTE_RUSH:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Vignette_Rush"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.1f;
		break;

	case VIGNETTE_DAMAGED:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Vignette_OnDamaged"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	default:
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Grandprix_Vignette::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_Vignette::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (VIGNETTE_RUSH == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
	}
	else if (VIGNETTE_DAMAGED == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

CUI_Grandprix_Vignette* CUI_Grandprix_Vignette::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIX_VIGNETTE eType)
{
	CUI_Grandprix_Vignette* pInstance = new CUI_Grandprix_Vignette(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_Vignette");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_Vignette::Clone(void* pArg)
{
	CUI_Grandprix_Vignette* pInstance = new CUI_Grandprix_Vignette(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_Vignette");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_Vignette::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

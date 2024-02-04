#include "stdafx.h"
#include "UI_Grandprix_RaderFrame.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Grandprix_RaderFrame::CUI_Grandprix_RaderFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADER eType)
	: CUI(pDevice, pContext, L"UI_Grandprix_RaderFrame")
	, m_eType(eType)
{
}

CUI_Grandprix_RaderFrame::CUI_Grandprix_RaderFrame(const CUI_Grandprix_RaderFrame& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Grandprix_RaderFrame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_RaderFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (UI_RADER::RADER_BACKGROUND == m_eType)
		m_fAlpha = 0.75f;
	else if (UI_RADER::RADER_CIRCLE == m_eType)
		m_fAlpha = 0.f; // Max´Â 0.9f

	m_bActive = false;

	return S_OK;
}

void CUI_Grandprix_RaderFrame::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == RADER_CIRCLE)
		{
			if (true == m_bNext)
			{
				m_fTimeAcc += fTimeDelta;

				if (0.1f < m_fTimeAcc)
				{
					m_fTimeAcc = 0.f;

					if (m_iTextureIndex >= 11)
						m_iTextureIndex = 0;
					else
						m_iTextureIndex++;

					m_bNext = false;
					m_bReverseAlpha = false;
				}
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_RaderFrame::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == RADER_CIRCLE)
		{
			if (false == m_bReverseAlpha)
			{
				m_fAlpha += fTimeDelta;
				if (0.9f <= m_fAlpha)
				{
					m_fAlpha = 0.9f;
					m_bReverseAlpha = true;
				}
			}
			else
			{
				m_fAlpha -= fTimeDelta;
				if (0.f >= m_fAlpha)
				{
					m_fAlpha = 0.f;
					m_bNext = true;
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Grandprix_RaderFrame::Render()
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

HRESULT CUI_Grandprix_RaderFrame::Ready_Components()
{
	if (m_eType == RADER_END)
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_eType == RADER_CIRCLE)
	{
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_Circle"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CUI_Grandprix_RaderFrame::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_RaderFrame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_eType == RADER_CIRCLE)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
	}

	return S_OK;
}

CUI_Grandprix_RaderFrame* CUI_Grandprix_RaderFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADER eType)
{
	CUI_Grandprix_RaderFrame* pInstance = new CUI_Grandprix_RaderFrame(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_RaderFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_RaderFrame::Clone(void* pArg)
{
	CUI_Grandprix_RaderFrame* pInstance = new CUI_Grandprix_RaderFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_RaderFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_RaderFrame::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

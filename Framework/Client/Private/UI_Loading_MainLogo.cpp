#include "stdafx.h"
#include "UI_Loading_MainLogo.h"
#include "GameInstance.h"

CUI_Loading_MainLogo::CUI_Loading_MainLogo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_MAINLOGOTYPE eType)
	: CUI(pDevice, pContext, L"UI_Loading_MainLogo")
	,m_eType(eType)
{
}

CUI_Loading_MainLogo::CUI_Loading_MainLogo(const CUI_Loading_MainLogo& rhs)
	: CUI(rhs)
	,m_eType(rhs.m_eType)
{
}

HRESULT CUI_Loading_MainLogo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_MainLogo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	switch (m_eType)
	{
	case MAINLOGO_TEXT:
		m_iTextureIndex = 0;
		break;

	case MAINLOGO_NUM:
		m_iTextureIndex = 1;
		break;
	}

	if (MAINLOGO_NUM == m_eType)
	{
		m_bFinish.resize(2);
		m_bFinish[0] = false;
		m_bFinish[1] = false;

		m_vOriginSize.x = m_tInfo.fCX;
		m_vOriginSize.y = m_tInfo.fCY;

		m_vMaxSize.x = m_vOriginSize.x * 1.5f;
		m_vMaxSize.y = m_vOriginSize.y * 1.5f;

		m_vSizeAcc.x = m_vOriginSize.x * 0.1;
		m_vSizeAcc.y = m_vOriginSize.y * 0.1;

		m_bResize = false;
		m_bRestart = true;
		m_fRestartTimeAcc = 0.f;
	}

	m_fAlpha = 1.f;

	return S_OK;
}

void CUI_Loading_MainLogo::Tick(_float fTimeDelta)
{
	if (0 > m_iTextureIndex || 1 < m_iTextureIndex)
		return;

	if (MAINLOGO_NUM == m_eType)
	{
		if (m_bRestart)
		{
			if (!m_bResize) // Resize가 완료되지 않은 상황이라면
			{
				if (!m_bFinish[1])
				{
					if (m_vMaxSize.y < m_tInfo.fCY)
					{
						m_tInfo.fCY = m_vMaxSize.y;
						m_bFinish[1] = true;
						//m_bResize = true;
					}
					else
						m_tInfo.fCY += fTimeDelta * m_vSizeAcc.y;

					m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
				}

				if (!m_bFinish[0])
				{
					if (m_vMaxSize.x < m_tInfo.fCX)
					{
						m_tInfo.fCX = m_vMaxSize.x;
						m_bFinish[0] = true;
						//m_bResize = true;
					}
					else
						m_tInfo.fCX += fTimeDelta * m_vSizeAcc.x;

					m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
				}
			}

			if (m_bFinish[1] && m_bFinish[0])
			{
				m_bResize = true;

				m_tInfo.fCX = m_vOriginSize.x;
				m_tInfo.fCY = m_vOriginSize.y;
				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			}
		}
	}

	__super::Tick(fTimeDelta);
}

void CUI_Loading_MainLogo::LateTick(_float fTimeDelta)
{
	if (0 > m_iTextureIndex || 1 < m_iTextureIndex)
		return;

	if (MAINLOGO_NUM == m_eType)
	{
		if (m_bResize)
		{
			if (m_fRestartTimeAcc > 2.f)
			{
				m_bRestart = true;
				m_bResize = false;
				m_fRestartTimeAcc = 0.f;

//				m_tInfo.fCX = m_vOriginSize.x;
//				m_tInfo.fCY = m_vOriginSize.y;
//				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

				m_bFinish[0] = false;
				m_bFinish[1] = false;
			}

			m_fRestartTimeAcc += fTimeDelta;
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Loading_MainLogo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading_MainLogo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Logos"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_MainLogo::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Loading_MainLogo::Bind_ShaderResources()
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

CUI_Loading_MainLogo * CUI_Loading_MainLogo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_MAINLOGOTYPE eType)
{
	CUI_Loading_MainLogo*	pInstance = new CUI_Loading_MainLogo(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_MainLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Loading_MainLogo::Clone(void* pArg)
{
	CUI_Loading_MainLogo*	pInstance = new CUI_Loading_MainLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_MainLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_MainLogo::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

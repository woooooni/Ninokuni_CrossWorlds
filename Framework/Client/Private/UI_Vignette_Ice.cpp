#include "stdafx.h"
#include "UI_Vignette_Ice.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Vignette_Ice::CUI_Vignette_Ice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Vignette_Ice")
{
}

CUI_Vignette_Ice::CUI_Vignette_Ice(const CUI_Vignette_Ice& rhs)
	: CUI(rhs)
{
}

void CUI_Vignette_Ice::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
		m_iTextureIndex = 0;
		m_fTimeAcc = 0.f;
		m_bIsFinished = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Vignette_Ice::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Vignette_Ice::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_iMaxTextureIndex = 4;

	return S_OK;
}

void CUI_Vignette_Ice::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (4 > m_iTextureIndex)
		{
			m_fTimeAcc += fTimeDelta;

			if (0.2f < m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				if (m_iMaxTextureIndex > m_iTextureIndex)
					m_iTextureIndex++;
			}
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_Vignette_Ice::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_EFFECT_BLEND, this);
	}
}

HRESULT CUI_Vignette_Ice::Render()
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

void CUI_Vignette_Ice::Decrease_TextureIndex()
{
	if (0 >= m_iTextureIndex)
		return;

	m_iMaxTextureIndex--;
	m_iTextureIndex--;
}

HRESULT CUI_Vignette_Ice::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Vignette_Ice"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Vignette_Ice::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Vignette_Ice::Bind_ShaderResources()
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

CUI_Vignette_Ice* CUI_Vignette_Ice::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Vignette_Ice* pInstance = new CUI_Vignette_Ice(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Vignette_Ice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Vignette_Ice::Clone(void* pArg)
{
	CUI_Vignette_Ice* pInstance = new CUI_Vignette_Ice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Vignette_Ice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Vignette_Ice::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

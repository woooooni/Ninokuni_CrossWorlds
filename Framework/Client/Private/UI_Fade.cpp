#include "stdafx.h"
#include "UI_Fade.h"
#include "GameInstance.h"

CUI_Fade::CUI_Fade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_VEIL eType)
	: CUI(pDevice, pContext, L"UI_Fade"), m_eVeilType(eType)
{
}

CUI_Fade::CUI_Fade(const CUI_Fade& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Fade::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Fade::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CUI_Fade::Tick(_float fTimeDelta)
{
	if (m_bIsComplete)
		return;

	if (!m_bIsFadeOut && m_fAlpha > 0.f)
	{
		m_fAlpha -= fTimeDelta / m_fFadeTime;

		if (m_fAlpha < 0.f)
			m_fAlpha = 0.f;
	}
	else if (!m_bIsFadeOut)
	{
		Set_Finish();
	}
	else if (m_bIsFadeOut && m_fAlpha < 1.f)
	{
		m_fAlpha += fTimeDelta / m_fFadeTime;
		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;

	}
	else if (m_bIsFadeOut)
	{
		Set_Finish();
	}

	__super::Tick(fTimeDelta);
}

void CUI_Fade::LateTick(_float fTimeDelta)
{
	if (m_bIsComplete)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Fade::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Fade::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eVeilType)
	{
	case UI_VEIL::VEIL_BLACK:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Veil_Black"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_VEIL::VEIL_WHITE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Veil_Black"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	

	return S_OK;
}

HRESULT CUI_Fade::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Fade::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_Fade* CUI_Fade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_VEIL eType)
{
	CUI_Fade* pInstance = new CUI_Fade(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Fade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Fade::Clone(void* pArg)
{
	CUI_Fade* pInstance = new CUI_Fade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Fade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Fade::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

#include "stdafx.h"
#include "UI_Loading_ProgressBar.h"
#include "GameInstance.h"

CUI_Loading_ProgressBar::CUI_Loading_ProgressBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PROGRESSBAR eType)
	: CUI(pDevice, pContext, L"UI_Loading_ProgressBar")
	, m_eType(eType)
{
}

CUI_Loading_ProgressBar::CUI_Loading_ProgressBar(const CUI_Loading_ProgressBar& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Loading_ProgressBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_ProgressBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_bFinish = false;

	return S_OK;
}

void CUI_Loading_ProgressBar::Tick(_float fTimeDelta)
{
	if (UIPROG_END == m_eType)
		return;

	if (UIPROG_BAR == m_eType)
	{
		if (!m_bFinish)
		{
			if (0.1f <= m_fLoadingProg && 0.3 > m_fLoadingProg)
			{
				m_fLoadingProg += fTimeDelta * 0.08f;
			}
			else if (0.3 <= m_fLoadingProg && 0.8 > m_fLoadingProg)
			{
				m_fLoadingProg += fTimeDelta * 0.3f;
			}
			else
				m_fLoadingProg += fTimeDelta * 0.5f;

			if (m_fLoadingProg > 1.f)
			{
				m_bFinish = true;
				m_fLoadingProg = 1.f;
			}
		}
	}

	__super::Tick(fTimeDelta);

}

void CUI_Loading_ProgressBar::LateTick(_float fTimeDelta)
{
	if (UIPROG_END == m_eType)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Loading_ProgressBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading_ProgressBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	switch (m_eType)
	{
	case UIPROG_BACK:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Progress_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iPass = 1;
		break;

	case UIPROG_BAR:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Progress_Bar"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iPass = 4;
		m_fLoadingProg = 0.1f;
		break;
	}

	return S_OK;
}

HRESULT CUI_Loading_ProgressBar::Ready_State()
{

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Loading_ProgressBar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (UIPROG_BAR == m_eType)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fLoadingProg, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Loading_ProgressBar* CUI_Loading_ProgressBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PROGRESSBAR eType)
{
	CUI_Loading_ProgressBar* pInstance = new CUI_Loading_ProgressBar(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Loading_ProgressBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Loading_ProgressBar::Clone(void* pArg)
{
	CUI_Loading_ProgressBar* pInstance = new CUI_Loading_ProgressBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Loading_ProgressBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_ProgressBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

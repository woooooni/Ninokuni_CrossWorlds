#include "stdafx.h"
#include "UI_Minigame_Curling_Base.h"
#include "GameInstance.h"

CUI_Minigame_Curling_Base::CUI_Minigame_Curling_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag)
	: CUI(pDevice, pContext, wstrTag)
{
}

CUI_Minigame_Curling_Base::CUI_Minigame_Curling_Base(const CUI_Minigame_Curling_Base& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Curling_Base::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Curling_Base::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);
}

void CUI_Minigame_Curling_Base::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Minigame_Curling_Base::Render()
{
	
	return S_OK;
}

HRESULT CUI_Minigame_Curling_Base::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Base::Ready_Default()
{
	/* Transform */
	{
		if (nullptr == m_pTransformCom)
			return E_FAIL;

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
	}

	/* Variable */
	{
		m_bActive	= false;
		m_fAlpha	= 1.f;

		m_iPass		= 1; // Alpha Blending
		m_vColor	= DirectX::Colors::White;
	}

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Base::Bind_ShaderResources()
{
	/* WVP */
	{
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;
	}

	return S_OK;
}


void CUI_Minigame_Curling_Base::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

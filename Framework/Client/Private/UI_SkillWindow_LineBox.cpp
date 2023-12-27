#include "stdafx.h"
#include "UI_SkillWindow_LineBox.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillWindow_LineBox::CUI_SkillWindow_LineBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SKILLWINDOW eType)
	: CUI(pDevice, pContext, L"UI_SkillWindow_LineBox")
	, m_eUIType(eType)
{
}

CUI_SkillWindow_LineBox::CUI_SkillWindow_LineBox(const CUI_SkillWindow_LineBox& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
{
}

void CUI_SkillWindow_LineBox::Set_ArrowPosition(_float fPositionY)
{
	if (SKWINDOW_ARROW != m_eUIType)
		return;

	if (nullptr == m_pTransformCom)
		return;

	m_tInfo.fY = fPositionY;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
}

HRESULT CUI_SkillWindow_LineBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillWindow_LineBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.85f;

	return S_OK;
}

void CUI_SkillWindow_LineBox::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eUIType == UI_SKILLWINDOW::SKWINDOW_ARROW)
		{
			if (!m_bAlpha)
			{
				m_fAlpha -= fTimeDelta * 1.2f;
				if (m_fAlpha <= 0.f)
				{
					m_bAlpha = true;
					m_fAlpha = 0.f;
				}
			}
			else
			{
				m_fAlpha += fTimeDelta * 1.2f;
				if (m_fAlpha >= 1.f)
				{
					m_bAlpha = false;
					m_fAlpha = 1.f;
				}
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillWindow_LineBox::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		if (m_eUIType == UI_SKILLWINDOW::SKWINDOW_DESC)
		{
			if (0 > m_iDescIndex || 6 < m_iDescIndex)
				return;
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_SkillWindow_LineBox::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_SkillWindow_LineBox::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	switch (m_eUIType)
	{
	case UI_SKILLWINDOW::SKWINDOW_SUBMENU:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_SubMenu"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_SKILLWINDOW::SKWINDOW_LINEBOX:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_LineBox"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_SKILLWINDOW::SKWINDOW_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Select_Arrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_SKILLWINDOW::SKWINDOW_TITLE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Titles"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_SKILLWINDOW::SKWINDOW_DESC:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Desc_Swordsman"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	
	return S_OK;
}

HRESULT CUI_SkillWindow_LineBox::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillWindow_LineBox::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_eUIType == UI_SKILLWINDOW::SKWINDOW_TITLE)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTitleIndex)))
			return E_FAIL;
	}
	else if (m_eUIType == UI_SKILLWINDOW::SKWINDOW_DESC)
	{
		//if (0 > m_iDescIndex || 6 < m_iDescIndex)
		//	return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iDescIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

CUI_SkillWindow_LineBox* CUI_SkillWindow_LineBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SKILLWINDOW eType)
{
	CUI_SkillWindow_LineBox* pInstance = new CUI_SkillWindow_LineBox(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillWindow_LineBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillWindow_LineBox::Clone(void* pArg)
{
	CUI_SkillWindow_LineBox* pInstance = new CUI_SkillWindow_LineBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillWindow_LineBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillWindow_LineBox::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

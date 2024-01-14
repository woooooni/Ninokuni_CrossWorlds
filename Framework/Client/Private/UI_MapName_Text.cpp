#include "stdafx.h"
#include "GameInstance.h"
#include "UI_MapName_Text.h"
#include "UI_Manager.h"

CUI_MapName_Text::CUI_MapName_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MapName_Text")
{
	m_eType = EVERMORE_SOUTH;
}

CUI_MapName_Text::CUI_MapName_Text(const CUI_MapName_Text& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_MapName_Text::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MapName_Text::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.9f;

	return S_OK;
}

void CUI_MapName_Text::Tick(_float fTimeDelta)
{
	if (MAPNAMETEXT_END == m_eType)
		return;

	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_MapName_Text::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MapName_Text::Render()
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

HRESULT CUI_MapName_Text::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_Text_Renewal"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MapName_Text::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MapName_Text::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
		return E_FAIL;

	return S_OK;
}


CUI_MapName_Text* CUI_MapName_Text::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MapName_Text* pInstance = new CUI_MapName_Text(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MapName_Text");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MapName_Text::Clone(void* pArg)
{
	CUI_MapName_Text* pInstance = new CUI_MapName_Text(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MapName_Text");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MapName_Text::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

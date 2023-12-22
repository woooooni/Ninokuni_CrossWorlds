#include "stdafx.h"
#include "UI_PopupQuest.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_PopupQuest::CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType)
	: CUI(pDevice, pContext, L"UI_PopupQuest")
	, m_eType(eType)
{
}

CUI_PopupQuest::CUI_PopupQuest(const CUI_PopupQuest& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_PopupQuest::Set_Active(_bool bActive)
{
	if (POPUPFRAME_TOP == m_eType || POPUPFRAME_BOTTOM == m_eType)
	{
		if (bActive)
		{
			m_fAppearProg = 0.f; // 초기화해준다.
		}
		else
		{

		}
	}
	else
	{

	}

	m_bActive = bActive;
}

HRESULT CUI_PopupQuest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PopupQuest::Initialize(void* pArg)
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

void CUI_PopupQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUESTPOPUP_END == m_eType)
			return;

		if (m_fAppearProg <= 1.f)
			m_fAppearProg += fTimeDelta * 10.f;

		__super::Tick(fTimeDelta);
	}
}

void CUI_PopupQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUESTPOPUP_END == m_eType)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PopupQuest::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(8);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_PopupQuest::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_PopupQuest::On_Mouse(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
}

void CUI_PopupQuest::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_PopupQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case POPUPFRAME_TOP:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case POPUPFRAME_BOTTOM:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case POPUPWINDOW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Window"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.4f;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_PopupQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_PopupQuest::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (POPUPFRAME_TOP == m_eType || POPUPFRAME_BOTTOM == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
	}
	else if (POPUPWINDOW == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fAppearProg, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_PopupQuest::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_PopupQuest* CUI_PopupQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PopupQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PopupQuest::Clone(void* pArg)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PopupQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PopupQuest::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

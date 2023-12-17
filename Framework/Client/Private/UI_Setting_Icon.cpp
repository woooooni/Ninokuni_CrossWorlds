#include "stdafx.h"
#include "UI_Setting_Icon.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Setting_Icon::CUI_Setting_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_ICONTYPE eType)
	: CUI(pDevice, pContext, L"UI_Setting_Icon")
	, m_eIconType(eType)
{
}

CUI_Setting_Icon::CUI_Setting_Icon(const CUI_Setting_Icon& rhs)
	: CUI(rhs)
	, m_eIconType(rhs.m_eIconType)
{
}

HRESULT CUI_Setting_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_Icon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	
	return S_OK;
}

void CUI_Setting_Icon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_Icon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Setting_Icon::Render()
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

void CUI_Setting_Icon::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Setting_Icon::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_Setting_Icon::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Setting_Icon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Button"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Setting_Icon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_Icon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eIconType))))
		return E_FAIL;

	return S_OK;
}

void CUI_Setting_Icon::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 이마진이 있는 상태, 이마진이 없는 상태를 나눈다.
	}
}

CUI_Setting_Icon* CUI_Setting_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_ICONTYPE eType)
{
	CUI_Setting_Icon* pInstance = new CUI_Setting_Icon(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_Icon::Clone(void* pArg)
{
	CUI_Setting_Icon* pInstance = new CUI_Setting_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

#include "stdafx.h"
#include "UI_InGame_Setting_RadioBtn.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_InGame_Setting_RadioBtn::CUI_InGame_Setting_RadioBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADIOGROUP eGroupType, UI_RADIOTYPE eType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_RadioBtn")
	, m_eBtnType(eType)
	, m_eGroupType(eGroupType)
{
}

CUI_InGame_Setting_RadioBtn::CUI_InGame_Setting_RadioBtn(const CUI_InGame_Setting_RadioBtn& rhs)
	: CUI(rhs)
	, m_eBtnType(rhs.m_eBtnType)
	, m_eGroupType(rhs.m_eGroupType)
{
}

HRESULT CUI_InGame_Setting_RadioBtn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_RadioBtn::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bUseMouse = true;
	m_bActive = true;

	return S_OK;
}

void CUI_InGame_Setting_RadioBtn::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_RadioBtn::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_InGame_Setting_RadioBtn::Render()
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

void CUI_InGame_Setting_RadioBtn::On_MouseEnter(_float fTimeDelta)
{

}

void CUI_InGame_Setting_RadioBtn::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			CUI_Manager::GetInstance()->Update_SettingGraphicRadio(m_eGroupType, m_eBtnType);
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_InGame_Setting_RadioBtn::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_InGame_Setting_RadioBtn::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_RadioBtn"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_InGame_Setting_RadioBtn::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_RadioBtn::Bind_ShaderResources()
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

CUI_InGame_Setting_RadioBtn* CUI_InGame_Setting_RadioBtn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADIOGROUP eGroupType, UI_RADIOTYPE eType)
{
	CUI_InGame_Setting_RadioBtn* pInstance = new CUI_InGame_Setting_RadioBtn(pDevice, pContext, eGroupType, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_RadioBtn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_RadioBtn::Clone(void* pArg)
{
	CUI_InGame_Setting_RadioBtn* pInstance = new CUI_InGame_Setting_RadioBtn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_RadioBtn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_RadioBtn::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

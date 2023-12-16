#include "stdafx.h"
#include "UI_Btn_WorldMapIcon.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Btn_WorldMapIcon::CUI_Btn_WorldMapIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_WORLDMAP_ICONTYPE eType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_eIconType(eType)
{
}

CUI_Btn_WorldMapIcon::CUI_Btn_WorldMapIcon(const CUI_Btn_WorldMapIcon& rhs)
	: CUI(rhs)
	, m_eIconType(rhs.m_eIconType)
	, m_iTextureIndex(rhs.m_iTextureIndex)
{
}

void CUI_Btn_WorldMapIcon::Set_Active(_bool bActive)
{
	if (bActive)
	{

	}

	m_bActive = bActive;
}

HRESULT CUI_Btn_WorldMapIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Btn_WorldMapIcon::Initialize(void* pArg)
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

void CUI_Btn_WorldMapIcon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Btn_WorldMapIcon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Btn_WorldMapIcon::Render()
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

void CUI_Btn_WorldMapIcon::On_MouseEnter(_float fTimeDelta)
{

}

void CUI_Btn_WorldMapIcon::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
		Key_Input(fTimeDelta);
}

void CUI_Btn_WorldMapIcon::On_MouseExit(_float fTimeDelta)
{

}

HRESULT CUI_Btn_WorldMapIcon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapIcon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_eIconType)
	{
	case ICON_EVERMORE:
		m_iTextureIndex = 0;
		break;

	case ICON_ICELAND:
		m_iTextureIndex = 1;
		break;

	case ICON_WITCHFOREST:
		m_iTextureIndex = 2;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Btn_WorldMapIcon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Btn_WorldMapIcon::Bind_ShaderResources()
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

void CUI_Btn_WorldMapIcon::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{

	}
}

CUI_Btn_WorldMapIcon* CUI_Btn_WorldMapIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_WORLDMAP_ICONTYPE eType)
{
	CUI_Btn_WorldMapIcon* pInstance = new CUI_Btn_WorldMapIcon(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Btn_WorldMapIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Btn_WorldMapIcon::Clone(void* pArg)
{
	CUI_Btn_WorldMapIcon* pInstance = new CUI_Btn_WorldMapIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Btn_WorldMapIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Btn_WorldMapIcon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

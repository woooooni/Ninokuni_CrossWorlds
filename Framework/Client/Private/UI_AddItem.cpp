#include "stdafx.h"
#include "UI_AddItem.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_AddItem::CUI_AddItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType)
	: CUI(pDevice, pContext, L"UI_AddItem")
	, m_eType(eType)
{
}

CUI_AddItem::CUI_AddItem(const CUI_AddItem& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_AddItem::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_AddItem::Initialize(void* pArg)
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

void CUI_AddItem::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUICKITEM_END == m_eType)
			return;

		__super::Tick(fTimeDelta);
	}
}

void CUI_AddItem::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUICKITEM_END == m_eType)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_AddItem::Render()
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

HRESULT CUI_AddItem::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_AddItem_Popups"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_AddItem::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_AddItem::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_AddItem* CUI_AddItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType)
{
	CUI_AddItem* pInstance = new CUI_AddItem(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_AddItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_AddItem::Clone(void* pArg)
{
	CUI_AddItem* pInstance = new CUI_AddItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_AddItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_AddItem::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

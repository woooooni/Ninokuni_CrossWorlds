#include "stdafx.h"
#include "UI_Inventory_Slot.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Inventory_Slot::CUI_Inventory_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INVENSLOT_TYPE eType)
	: CUI(pDevice, pContext, L"UI_Inventory_Slot")
	, m_eType(eType)
{
}

CUI_Inventory_Slot::CUI_Inventory_Slot(const CUI_Inventory_Slot& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Inventory_Slot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Inventory_Slot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.9f;
	m_bUseMouse = true;

	return S_OK;
}

void CUI_Inventory_Slot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Inventory_Slot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Inventory_Slot::Render()
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

HRESULT CUI_Inventory_Slot::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_InvenSlots"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Inventory_Slot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Inventory_Slot::Bind_ShaderResources()
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

CUI_Inventory_Slot* CUI_Inventory_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INVENSLOT_TYPE eType)
{
	CUI_Inventory_Slot* pInstance = new CUI_Inventory_Slot(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Inventory_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_Slot::Clone(void* pArg)
{
	CUI_Inventory_Slot* pInstance = new CUI_Inventory_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Inventory_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

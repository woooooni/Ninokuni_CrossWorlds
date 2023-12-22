#include "stdafx.h"
#include "UI_QuickSlot_Item.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_QuickSlot_Item::CUI_QuickSlot_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType)
	: CUI(pDevice, pContext, L"UI_QuickSlot_Item")
	, m_eType(eType)
{
}

CUI_QuickSlot_Item::CUI_QuickSlot_Item(const CUI_QuickSlot_Item& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_QuickSlot_Item::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_QuickSlot_Item::Initialize(void* pArg)
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

void CUI_QuickSlot_Item::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUICKITEM_END == m_eType)
			return;

//		if (SKILL_THIRD == m_eType)
//		{
//			m_tInfo.fCX = 256.f * 0.26f;
//			m_tInfo.fCY = 256.f * 0.26f;
//			m_tInfo.fX = g_iWinSizeX - 154.f;
//			m_tInfo.fY = g_iWinSizeY - 253.f;
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
//		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_QuickSlot_Item::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUICKITEM_END == m_eType)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_QuickSlot_Item::Render()
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

void CUI_QuickSlot_Item::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_QuickSlot_Item::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_QuickSlot_Item::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_QuickSlot_Item::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuickSlot_Item_Bg"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_QuickSlot_Item::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_QuickSlot_Item::Bind_ShaderResources()
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

void CUI_QuickSlot_Item::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_QuickSlot_Item* CUI_QuickSlot_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType)
{
	CUI_QuickSlot_Item* pInstance = new CUI_QuickSlot_Item(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_QuickSlot_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuickSlot_Item::Clone(void* pArg)
{
	CUI_QuickSlot_Item* pInstance = new CUI_QuickSlot_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_QuickSlot_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_QuickSlot_Item::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

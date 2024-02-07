#include "stdafx.h"
#include "UI_AddItem.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_AddItem::CUI_AddItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_AddItem")
{
	m_eType = UI_QUICKSLOT_ITEM::QUICKITEM_END;
}

CUI_AddItem::CUI_AddItem(const CUI_AddItem& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_AddItem::Set_Position(_int iOrder)
{
	m_tInfo.fCX = 400.f * 0.5f;
	m_tInfo.fCY = 93.f * 0.5f;
	m_tInfo.fX = 1270.f;

	switch (iOrder)
	{
	case 0:
		m_tInfo.fY = 100.f;
		break;

	case 1: // y로 48씩 offset
		m_tInfo.fY = 148.f;
		break;

	case 2:
		m_tInfo.fY = 196.f;
		break;

	case 3:
		m_tInfo.fY = 244.f;
		break;

	default:
		break;
	}

	m_vTextPos.x = m_tInfo.fX - 50.f;
	m_vTextPos.y = m_tInfo.fY - 10.f;

	m_vDisappearPos.x = m_tInfo.fX + m_tInfo.fCX;
	m_vDisappearPos.y = m_tInfo.fY - m_tInfo.fCY;

	m_bDisappear = false;
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

}

HRESULT CUI_AddItem::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_AddItem::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_tItemDesc = *((UIITEM_DESC*)pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	switch (m_tItemDesc.eCode)
	{
	case ITEM_CODE::CONSUMPSION_GOLD:
		m_strText = TEXT("골드 X ") + to_wstring(m_tItemDesc.iCount);
		m_iTextureIndex = 0;
		break;

	case ITEM_CODE::CONSUMPSION_ENERGY:
		m_strText = TEXT("에너지 드링크 X ") + to_wstring(m_tItemDesc.iCount);
		m_iTextureIndex = 1;
		break;

	case ITEM_CODE::CONSUMPSION_EXP:
		m_iTextureIndex = 2;
		break;

	case ITEM_CODE::CONSUMPSION_MILK:
		m_iTextureIndex = 3;
		break;

	case ITEM_CODE::CONSUMPSION_PUDDING:
		m_iTextureIndex = 4;
		break;

	case ITEM_CODE::CONSUMPSION_BREAD:
		m_iTextureIndex = 5;
		break;

	default:
		return E_FAIL;
		break;
	}

	m_bActive = true;

	return S_OK;
}

void CUI_AddItem::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		if (QUICKITEM_END == m_eType)
//			return;

		if (false == m_bDisappear)
		{
			m_fTimeAcc += fTimeDelta;

			if (m_fTimeAcc > 1.f)
			{
				m_bDisappear = true;
				m_fTimeAcc = 0.f;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_AddItem::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		if (QUICKITEM_END == m_eType)
//			return;
		if (true == m_bDisappear)
		{
			m_fAlpha -= fTimeDelta * 10.f;

			if (m_fAlpha <= 0.f)
			{
				Set_Active(false);
				Set_Dead(true);
			}

			if (m_tInfo.fX < m_vDisappearPos.x)
				m_tInfo.fX += fTimeDelta * 500.f;

			if (m_tInfo.fY > m_vDisappearPos.y)
				m_tInfo.fY -= fTimeDelta * 100.f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
		else
		{
			CRenderer::TEXT_DESC TextDesc = {};
			TextDesc.strFontTag = L"Default_Bold";
			TextDesc.strText = m_strText;
			TextDesc.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
			TextDesc.vPosition = _float2(m_vTextPos.x - 1.f, m_vTextPos.y);
			TextDesc.vScale = Vec2(0.45f, 0.45f);
			m_pRendererCom->Add_Text(TextDesc);
			TextDesc.vPosition = _float2(m_vTextPos.x + 1.f, m_vTextPos.y);
			m_pRendererCom->Add_Text(TextDesc);
			TextDesc.vPosition = _float2(m_vTextPos.x, m_vTextPos.y - 1.f);
			m_pRendererCom->Add_Text(TextDesc);
			TextDesc.vPosition = _float2(m_vTextPos.x, m_vTextPos.y + 1.f);
			m_pRendererCom->Add_Text(TextDesc);

			// Default Font
			TextDesc.vColor = Vec4(0.722f, 0.698f, 0.584f, 1.f);
			TextDesc.vPosition = m_vTextPos;
			TextDesc.vScale = Vec2(0.45f, 0.45f);
			m_pRendererCom->Add_Text(TextDesc);
		}

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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_AddItem* CUI_AddItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_AddItem* pInstance = new CUI_AddItem(pDevice, pContext);

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

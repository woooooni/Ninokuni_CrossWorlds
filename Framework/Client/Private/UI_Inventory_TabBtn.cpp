#include "stdafx.h"
#include "UI_Inventory_TabBtn.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Inventory_TabBtn::CUI_Inventory_TabBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INVENBTN_TYPE eButtonType, UI_INVENTABBTN eUIType)
	: CUI(pDevice, pContext, L"UI_Inventory_TabBtn")
	, m_eUIType(eUIType)
	, m_eButtonType(eButtonType)
{
}

CUI_Inventory_TabBtn::CUI_Inventory_TabBtn(const CUI_Inventory_TabBtn& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
	, m_eButtonType(rhs.m_eButtonType)
{
}

void CUI_Inventory_TabBtn::Set_Active(_bool bActive)
{
	if (INVENTABBTN_END == m_eUIType)
		return;

	if (m_eButtonType == INVENBTN_CLICKED)
	{
		m_bAlpha = false;
		m_fFXAlpha = 0.f;
		m_fTimeAcc = 0.f;

		m_bResize = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Inventory_TabBtn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Inventory_TabBtn::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_bAlpha = false;

	if (m_eButtonType == UI_INVENBTN_TYPE::INVENBTN_UNCLICKED)
		m_iPass = 1;
	else if (m_eButtonType == UI_INVENBTN_TYPE::INVENBTN_CLICKED)
	{
		m_iPass = 13;
		m_fOriginfCY = m_tInfo.fCY;
		m_fMaxfCY = m_tInfo.fCY + 8.f;

		m_fOriginfY = m_tInfo.fY;
		m_fMaxfY = m_tInfo.fY - (m_fMaxfCY - m_fOriginfCY) * 0.5f;
	}

	return S_OK;
}

void CUI_Inventory_TabBtn::Tick(_float fTimeDelta)
{
	if (INVENTABBTN_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_eButtonType == UI_INVENBTN_TYPE::INVENBTN_CLICKED)
		{
			if (m_bResize) // 전부다 끝나면 알파값을 조정한다.
			{
				if (!m_bAlpha)
				{
					m_fFXAlpha -= fTimeDelta;

					if (m_fFXAlpha <= 0.f)
					{
						m_fFXAlpha = 0.f;
						m_bAlpha = true;
					}

				}
				else
				{
					m_fFXAlpha += fTimeDelta;

					if (m_fFXAlpha >= 1.f)
					{
						m_fFXAlpha = 1.f;
						m_bAlpha = false;
					}
				}
			}

		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_Inventory_TabBtn::LateTick(_float fTimeDelta)
{
	if (INVENTABBTN_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_eButtonType == UI_INVENBTN_TYPE::INVENBTN_CLICKED)
		{
			if (!m_bResize)
			{
				m_fTimeAcc += fTimeDelta;

				m_tInfo.fCY = m_fMaxfCY;
				m_tInfo.fY = m_fMaxfY;
				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.1f, 1.f));

				if (m_fTimeAcc > 0.1f)
				{
					m_fFXAlpha = 0.5f;

					m_tInfo.fCY = m_fOriginfCY;
					m_tInfo.fY = m_fOriginfY;
					m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
					m_pTransformCom->Set_State(CTransform::STATE_POSITION,
						XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.1f, 1.f));

					m_bResize = true;
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Inventory_TabBtn::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Inventory_TabBtn::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_Inventory_TabBtn::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			if (m_eButtonType == INVENBTN_UNCLICKED)
				CUI_Manager::GetInstance()->Update_InvenBtnState(_uint(m_eUIType));
		}
	}
}

void CUI_Inventory_TabBtn::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

HRESULT CUI_Inventory_TabBtn::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eButtonType)
	{
	case UI_INVENBTN_TYPE::INVENBTN_UNCLICKED:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_UnSelectedTab"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_INVENBTN_TYPE::INVENBTN_CLICKED:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_SelectedTab"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_TabFXTexture"),
			TEXT("Com_FXTexture"), (CComponent**)&m_pFXTexture)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Inventory_TabBtn::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.1f, 1.f));

	return S_OK;
}

HRESULT CUI_Inventory_TabBtn::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eUIType))))
		return E_FAIL;

	if (INVENBTN_CLICKED == m_eButtonType)
	{
		if (nullptr == m_pFXTexture)
			return E_FAIL;

		if (FAILED(m_pFXTexture->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_FXAlpha", &m_fFXAlpha, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Inventory_TabBtn* CUI_Inventory_TabBtn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INVENBTN_TYPE eButtonType, UI_INVENTABBTN eUIType)
{
	CUI_Inventory_TabBtn* pInstance = new CUI_Inventory_TabBtn(pDevice, pContext, eButtonType, eUIType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Inventory_TabBtn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_TabBtn::Clone(void* pArg)
{
	CUI_Inventory_TabBtn* pInstance = new CUI_Inventory_TabBtn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Inventory_TabBtn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_TabBtn::Free()
{
	__super::Free();

	if (INVENBTN_CLICKED == m_eButtonType)
		Safe_Release(m_pFXTexture);

	Safe_Release(m_pTextureCom);
}
